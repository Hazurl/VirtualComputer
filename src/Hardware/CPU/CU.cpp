#include <vphaz/Hardware/CPU/CU.h>
#include <iostream>
#include <vphaz/Bin.h>

BEGIN_NS

CU::CU(MemoryControlFlow32* ram, Processable* alu, Processable* out, MemoryControlFlow32* reg_mem, MemoryControlFlow8* reg_flag, Bus8 * bus_flag, 
	   MemoryControlFlow32* reg_instr, Bus32* bus_instr, MemoryControlFlow32 * reg_instr_addr, Bus8 * bus_alu, MemoryControlFlow32 * reg_alu_res,
	   MemoryControlFlow32 * reg_alu_tmp, MemoryControlFlow32 * reg_a, MemoryControlFlow32 * reg_b, MemoryControlFlow32 * reg_c,
	   MemoryControlFlow32* reg_seg_instr, MemoryControlFlow32* reg_seg_stack, MemoryControlFlow32* reg_stack_ptr)
  : alu(alu),
	out(out),
	ram(ram),
	reg_mem(reg_mem),
	reg_flag(reg_flag),
	bus_flag(bus_flag),
	reg_instr(reg_instr),
	bus_instr(bus_instr),
	reg_instr_addr(reg_instr_addr),
	bus_alu(bus_alu),
	reg_alu_res(reg_alu_res),
	reg_alu_tmp(reg_alu_tmp),
	reg_a(reg_a),
	reg_b(reg_b),
	reg_c(reg_c),
	reg_seg_instr(reg_seg_instr),
	reg_seg_stack(reg_seg_stack),
	reg_stack_ptr(reg_stack_ptr)
{

}

void CU::process() {
	fetch();

	dword instr = bus_instr->extract_32();
	word instrp = bin::msb(instr);
	byte instrs = bin::msb(bin::lsb(instr));
	byte instrt = bin::lsb(bin::lsb(instr));

//	std::cout << "IS: " << instr << " -> " << instrp << "-" << (int)instrs << "-" << (int)instrt << std::endl;

	InstrSet is = static_cast<InstrSet>(instrp);
	InstrTarget t0 = static_cast<InstrTarget>(instrs);
	InstrTarget t1 = static_cast<InstrTarget>(instrt);

	switch(is) {
	case InstrSet::Jmp: is_jmp(t0, t1, true); break;
	case InstrSet::Jmpz: is_jmp( t0, t1, is_flag(ALUFlag::Zero) ); break;
	case InstrSet::Jmpnz: is_jmp(t0, t1,  is_not_flag(ALUFlag::Zero) ); break;
	case InstrSet::Jmpg: is_jmp(t0, t1,  is_flag(ALUFlag::Comp) ); break;
	case InstrSet::Jmpgz: is_jmp(t0, t1,  is_flag(ALUFlag::Comp) || is_flag(ALUFlag::Zero) ); break;
	case InstrSet::Jmpl: is_jmp(t0, t1,  is_not_flag(ALUFlag::Comp) && is_not_flag(ALUFlag::Zero) ); break;
	case InstrSet::Jmplz: is_jmp(t0, t1,  is_not_flag(ALUFlag::Comp) ); break;

	case InstrSet::Push: is_push(t0, t1); break;
	case InstrSet::Pop: is_pop(t0, t1); break;
	case InstrSet::Move: is_move(t0, t1); break;
	case InstrSet::Out: is_out(t0, t1); break;

	case InstrSet::Defer: is_defer(t0, t1); break;
	case InstrSet::Add: is_add(t0, t1); break;
	case InstrSet::Sub: is_sub(t0, t1); break;
	case InstrSet::Mul: is_mul(t0, t1); break;
	case InstrSet::Div: is_div(t0, t1); break;
	case InstrSet::Mod: is_mod(t0, t1); break;
	case InstrSet::LShift: is_lshift(t0, t1); break;
	case InstrSet::RShift: is_rshift(t0, t1); break;
	case InstrSet::Comp: is_comp(t0, t1); break;
	case InstrSet::Inc: is_inc(t0, t1); break;
	case InstrSet::Dec: is_dec(t0, t1); break;
	case InstrSet::Neg:	is_neg(t0, t1); break;
	case InstrSet::Comp0: is_comp0(t0, t1); break;

	case InstrSet::Halt: halted = true; break;

	default: 
		throw InstrSetUnknown(instr);
	}
}

void CU::processALU(InstrTarget t0, InstrTarget t1, ALUInstrSet b8, ALUInstrSet b16, ALUInstrSet b32, bool result) {
	if(is_reg(t1)) {
		target_write(t1);
	} else if(t1 == InstrTarget::Addr) {
		instrAddrForward();
		fetch();
		reg_alu_tmp->read_write32();
		reg_seg_instr->write32();
		bus_alu->bind_8(bytev(ALUInstrSet::Add32));
		alu->process();
		reg_alu_res->read_write32();
		reg_mem->read_write32();
		ram->write32();
	} else {
		throw InstrSetUnknown(static_cast<dword>(t1));
	}
	switch(target_size(t1)) {
	case 32:
		reg_alu_tmp->read_write32();
		break;
	case 16:
		reg_alu_tmp->read_write16();
		break;
	case 8:
		reg_alu_tmp->read_write8();
		break;
	}
	processALU(t0, b8, b16, b32, result);
}

void CU::processALU(InstrTarget t0, ALUInstrSet b8, ALUInstrSet b16, ALUInstrSet b32, bool result) {
	if(is_reg(t0)) {
		target_write(t0);
	} else {
		throw InstrSetUnknown(static_cast<dword>(t0));
	}
	switch(target_size(t0)) {
	case 32:
		bus_alu->bind_8(bytev(b32));
		alu->process();
		reg_flag->read_write8();
		reg_alu_res->read_write32();
		break;
	case 16:
		bus_alu->bind_8(bytev(b16));
		alu->process();
		reg_flag->read_write8();
		reg_alu_res->read_write16();
		break;
	case 8:
		bus_alu->bind_8(bytev(b8));
		alu->process();
		reg_flag->read_write8();
		reg_alu_res->read_write8();
		break;
	}
	if(result)
		target_read(t0);
	instrAddrForward();
}

void CU::instrAddrForward() {
	reg_instr_addr->write32();
	bus_alu->bind_8(bytev(ALUInstrSet::Inc32));
	alu->process(); reg_alu_res->read_write32();
	alu->process(); reg_alu_res->read_write32();
	alu->process();	reg_alu_res->read_write32();
	alu->process();	reg_alu_res->read_write32();
	reg_instr_addr->read32();
}

void CU::fetch() {
	reg_instr_addr->write32();
	reg_mem->read_write32();
	ram->write32();
	reg_instr->read_write32();
}

bool CU::is_flag(ALUFlag f) {
	return (bus_flag->extract_8() & bytev(f)) != 0;
}

bool CU::is_not_flag(ALUFlag f) {
	return !is_flag(f);
}

bool CU::is_reg(InstrTarget t) {
	switch(t) {
	case vphaz::InstrTarget::Addr:
	case vphaz::InstrTarget::Value:
		return false;
	default:
		return true;
	}
}

void CU::target_read(InstrTarget t) {
	switch(t) {
	case vphaz::InstrTarget::a32:  reg_a->read32(); break;
	case vphaz::InstrTarget::b32:  reg_b->read32(); break;
	case vphaz::InstrTarget::c32:  reg_c->read32(); break;
	case vphaz::InstrTarget::a16:  reg_a->read16(); break;
	case vphaz::InstrTarget::b16:  reg_b->read16(); break;
	case vphaz::InstrTarget::c16:  reg_c->read16(); break;
	case vphaz::InstrTarget::a16h: reg_a->read16h(); break;
	case vphaz::InstrTarget::b16h: reg_b->read16h(); break;
	case vphaz::InstrTarget::c16h: reg_c->read16h(); break;
	case vphaz::InstrTarget::a8:   reg_a->read8(); break;
	case vphaz::InstrTarget::b8:   reg_b->read8(); break;
	case vphaz::InstrTarget::c8:   reg_c->read8(); break;
	case vphaz::InstrTarget::a8hh: reg_a->read8hh(); break;
	case vphaz::InstrTarget::b8hh: reg_b->read8hh(); break;
	case vphaz::InstrTarget::c8hh: reg_c->read8hh(); break;
	case vphaz::InstrTarget::a8lh: reg_a->read8h(); break;
	case vphaz::InstrTarget::b8lh: reg_b->read8h(); break;
	case vphaz::InstrTarget::c8lh: reg_c->read8h(); break;
	case vphaz::InstrTarget::a8hl: reg_a->read8hl(); break;
	case vphaz::InstrTarget::b8hl: reg_b->read8hl(); break;
	case vphaz::InstrTarget::c8hl: reg_c->read8hl(); break;
	default:
		throw InstrSetUnknown(static_cast<dword>(t));
	}
}

void CU::target_write(InstrTarget t) {
	switch(t) {
	case vphaz::InstrTarget::a32:  reg_a->write32(); break;
	case vphaz::InstrTarget::b32:  reg_b->write32(); break;
	case vphaz::InstrTarget::c32:  reg_c->write32(); break;
	case vphaz::InstrTarget::a16:  reg_a->write16(); break;
	case vphaz::InstrTarget::b16:  reg_b->write16(); break;
	case vphaz::InstrTarget::c16:  reg_c->write16(); break;
	case vphaz::InstrTarget::a16h: reg_a->write16h(); break;
	case vphaz::InstrTarget::b16h: reg_b->write16h(); break;
	case vphaz::InstrTarget::c16h: reg_c->write16h(); break;
	case vphaz::InstrTarget::a8:   reg_a->write8(); break;
	case vphaz::InstrTarget::b8:   reg_b->write8(); break;
	case vphaz::InstrTarget::c8:   reg_c->write8(); break;
	case vphaz::InstrTarget::a8hh: reg_a->write8hh(); break;
	case vphaz::InstrTarget::b8hh: reg_b->write8hh(); break;
	case vphaz::InstrTarget::c8hh: reg_c->write8hh(); break;
	case vphaz::InstrTarget::a8lh: reg_a->write8h(); break;
	case vphaz::InstrTarget::b8lh: reg_b->write8h(); break;
	case vphaz::InstrTarget::c8lh: reg_c->write8h(); break;
	case vphaz::InstrTarget::a8hl: reg_a->write8hl(); break;
	case vphaz::InstrTarget::b8hl: reg_b->write8hl(); break;
	case vphaz::InstrTarget::c8hl: reg_c->write8hl(); break;
	default:
		throw InstrSetUnknown(static_cast<dword>(t));
	}
}

ubyte CU::target_size(InstrTarget t) {
	switch(t) {
	case vphaz::InstrTarget::a16:
	case vphaz::InstrTarget::b16:
	case vphaz::InstrTarget::c16:
	case vphaz::InstrTarget::a16h:
	case vphaz::InstrTarget::b16h:
	case vphaz::InstrTarget::c16h:
		return 16;
	case vphaz::InstrTarget::a8:
	case vphaz::InstrTarget::b8:
	case vphaz::InstrTarget::c8:
	case vphaz::InstrTarget::a8hh:
	case vphaz::InstrTarget::b8hh:
	case vphaz::InstrTarget::c8hh:
	case vphaz::InstrTarget::a8lh:
	case vphaz::InstrTarget::b8lh:
	case vphaz::InstrTarget::c8lh:
	case vphaz::InstrTarget::a8hl:
	case vphaz::InstrTarget::b8hl:
	case vphaz::InstrTarget::c8hl:
		return 8;
	case vphaz::InstrTarget::a32:
	case vphaz::InstrTarget::b32:
	case vphaz::InstrTarget::c32:
	case vphaz::InstrTarget::Addr:
	case vphaz::InstrTarget::Value:
		return 32;
	default:
		throw InstrSetUnknown(static_cast<dword>(t));
	}
}

#define ALU_IS(n) ALUInstrSet::n ## 8, ALUInstrSet::n ## 16, ALUInstrSet::n ## 32 

void CU::is_defer(InstrTarget t0, InstrTarget t1) {
	target_write(t0);
	reg_alu_tmp->read_write32();
	reg_seg_instr->write32();
	bus_alu->bind_8(bytev(ALUInstrSet::Add32));
	alu->process();
	reg_alu_res->read_write32();
	reg_mem->read_write32();
	switch(target_size(t1)) {
	case 32:
		ram->write32();
		break;
	case 16:
		ram->write16();
		break;
	case 8:
		ram->write8();
		break;
	}
	target_read(t1);
	instrAddrForward();
}

void CU::is_comp(InstrTarget t0, InstrTarget t1) {
	processALU(t0, t1, ALU_IS(Comp), false);
}

void CU::is_comp0(InstrTarget t0, InstrTarget) {
	processALU(t0, ALU_IS(Comp0), false);
}

void CU::is_add(InstrTarget t0, InstrTarget t1) {
	processALU(t0, t1, ALU_IS(Add));
}

void CU::is_sub(InstrTarget t0, InstrTarget t1) {
	processALU(t0, t1, ALU_IS(Sub));
}

void CU::is_mul(InstrTarget t0, InstrTarget t1) {
	processALU(t0, t1, ALU_IS(Mul));
}

void CU::is_div(InstrTarget t0, InstrTarget t1) {
	processALU(t0, t1, ALU_IS(Div));
}

void CU::is_mod(InstrTarget t0, InstrTarget t1) {
	processALU(t0, t1, ALU_IS(Mod));
}

void CU::is_inc(InstrTarget t0, InstrTarget) {
	processALU(t0, ALU_IS(Inc));
}

void CU::is_dec(InstrTarget t0, InstrTarget) {
	processALU(t0, ALU_IS(Dec));
}

void CU::is_neg(InstrTarget t0, InstrTarget) {
	processALU(t0, ALU_IS(Neg));
}

void CU::is_lshift(InstrTarget t0, InstrTarget t1) {
	processALU(t0, t1, ALU_IS(LShift));
}

void CU::is_rshift(InstrTarget t0, InstrTarget t1) {
	processALU(t0, t1, ALU_IS(RShift));
}

void CU::is_push(InstrTarget t0, InstrTarget) {
	// write to addr bus
	reg_seg_stack->write32();
	reg_mem->read_write32();
	// inc stack_ptr
	bus_alu->bind_8(bytev(ALUInstrSet::Inc32));
	alu->process();	reg_alu_res->read_write32();
	alu->process();	reg_alu_res->read_write32();
	alu->process();	reg_alu_res->read_write32();
	alu->process();	reg_alu_res->read_write32();
	reg_seg_stack->read32();
	// write to ram
	target_write(t0);
	ram->read32();
	instrAddrForward();
}

void CU::is_pop(InstrTarget t0, InstrTarget) {
	// dec stack_ptr
	reg_seg_stack->write32();
	bus_alu->bind_8(bytev(ALUInstrSet::Dec32));
	alu->process();	reg_alu_res->read_write32();
	alu->process();	reg_alu_res->read_write32();
	alu->process();	reg_alu_res->read_write32();
	alu->process();	reg_alu_res->read_write32();
	reg_seg_stack->read32();
	// write to addr bus
	reg_mem->read_write32();
	// read from ram
	ram->write32();
	target_read(t0);
	instrAddrForward();
}

void CU::is_move(InstrTarget t1, InstrTarget t0) { // move from t1 to t0
	if(is_reg(t1)) {
		if (is_reg(t0)) {
			target_write(t0);
			target_read(t1);
		} else if (t0 == InstrTarget::Value) {
			instrAddrForward();
			fetch();
			target_read(t1);
		} else if (t0 == InstrTarget::Addr) {
			instrAddrForward();
			fetch();
			reg_alu_tmp->read_write32();
			reg_seg_instr->write32();
			bus_alu->bind_8(bytev(ALUInstrSet::Add32));
			alu->process();
			reg_alu_res->read_write32();
			reg_mem->read_write32();
			switch(target_size(t1)) {
			case 32:
				ram->write32();
				break;
			case 16:
				ram->write16();
				break;
			case 8:
				ram->write8();
				break;
			}
			target_read(t1);
		}
	} else if(t1 == InstrTarget::Addr && is_reg(t0)) {
		instrAddrForward();
		fetch();
		reg_alu_tmp->read_write32();
		reg_seg_instr->write32();
		bus_alu->bind_8(bytev(ALUInstrSet::Add32));
		alu->process();
		reg_alu_res->read_write32();
		reg_mem->read_write32();
		target_write(t0);
		switch(target_size(t0)) {
		case 32:
			ram->read32();
			break;
		case 16:
			ram->read16();
			break;
		case 8:
			ram->read8();
			break;
		}
	} else {
		throw InstrSetUnknown(static_cast<word>(t0) << 16 | static_cast<word>(t1));
	}
	instrAddrForward();
}

void CU::is_out(InstrTarget t0, InstrTarget) {
	if(is_reg(t0)) {
		target_write(t0);
		out->process();
	} else if(t0 == InstrTarget::Addr) {
		instrAddrForward();
		fetch();
		reg_alu_tmp->read_write32();
		reg_seg_instr->write32();
		bus_alu->bind_8(bytev(ALUInstrSet::Add32));
		alu->process();
		reg_alu_res->read_write32();
		reg_mem->read_write32();
		ram->write32();
		out->process();
	} else if(t0 == InstrTarget::Value) {
		instrAddrForward();
		fetch();
		out->process();
	} else {
		throw InstrSetUnknown(static_cast<dword>(t0));
	}
	instrAddrForward();
}

void CU::is_jmp(InstrTarget t0, InstrTarget, bool value) {
	instrAddrForward();
	if(t0 == InstrTarget::Value) {
		if(value) {
			fetch();
			reg_alu_tmp->read_write32();
			reg_seg_instr->write32();
			bus_alu->bind_8(bytev(ALUInstrSet::Add32));
			alu->process();
			reg_alu_res->read_write32();
			reg_instr_addr->read32();
		} else {
			instrAddrForward();
		}
	} else if (t0 == InstrTarget::Addr) {
		if(value) {
			fetch();
			reg_alu_tmp->read_write32();
			reg_seg_instr->write32();
			bus_alu->bind_8(bytev(ALUInstrSet::Add32));
			alu->process();
			reg_alu_res->read_write32();
			reg_mem->read_write32();
			ram->write32();
			reg_instr_addr->read32();
		} else {
			instrAddrForward();
		}		
	} else {
		throw InstrSetUnknown(static_cast<dword>(t0));
	}
}

END_NS