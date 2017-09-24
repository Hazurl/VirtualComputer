#include "stdafx.h"
#include "CU.h"
#include <iostream>

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

	MemoryControlFlow32* reg_0 = nullptr;
	MemoryControlFlow32* reg_1 = nullptr;
	bool is_value = false;
	bool is_address = false;
	bool is_relative = false;

	switch(static_cast<InstrSetSecondary>(bus_instr->extract_lh())) {
	case InstrSetSecondary::Ra: reg_0 = reg_a; break;
	case InstrSetSecondary::Rb: reg_0 = reg_b; break;
	case InstrSetSecondary::Rc: reg_0 = reg_c; break;
	case InstrSetSecondary::Rab: reg_0 = reg_a; reg_1 = reg_b; break;
	case InstrSetSecondary::Rba: reg_0 = reg_b; reg_1 = reg_a; break;
	case InstrSetSecondary::Rac: reg_0 = reg_a; reg_1 = reg_c; break;
	case InstrSetSecondary::Rca: reg_0 = reg_c; reg_1 = reg_a; break;
	case InstrSetSecondary::Rbc: reg_0 = reg_b; reg_1 = reg_c; break;
	case InstrSetSecondary::Rcb: reg_0 = reg_c; reg_1 = reg_b; break;
	case InstrSetSecondary::Addr: is_address = true; break;
	case InstrSetSecondary::RelAddr: is_address = is_relative = true; break;
	case InstrSetSecondary::Value: is_value = true; break;

	default:
		std::cout << "Oups (unknown code: " << (int)bus_instr->extract_l() << ")" << std::endl;
		std::cin.get();
		exit(1);
	}

	switch(static_cast<InstrSet>(bus_instr->extract_h())) {
	case InstrSet::Jmp:
		instrAddrForward();
		jmp_if(true);
		break;
	case InstrSet::Jmpz:
		instrAddrForward();
		jmp_if( is_flag(ALUFlag::Zero) );
		break;
	case InstrSet::Jmpnz:
		instrAddrForward();
		jmp_if( is_not_flag(ALUFlag::Zero) );
		break;
	case InstrSet::Jmpg:
		instrAddrForward();
		jmp_if( is_flag(ALUFlag::Comp) );
		break;
	case InstrSet::Jmpgz:
		instrAddrForward();
		jmp_if( is_flag(ALUFlag::Comp) || is_flag(ALUFlag::Zero) );
		break;
	case InstrSet::Jmpl:
		instrAddrForward();
		jmp_if( is_not_flag(ALUFlag::Comp) && is_not_flag(ALUFlag::Zero) );
		break;
	case InstrSet::Jmplz:
		instrAddrForward();
		jmp_if( is_not_flag(ALUFlag::Comp) );
		break;

	case InstrSet::Move:
		reg_a->write32();
		reg_b->read32();
		instrAddrForward();
		break;

	case InstrSet::Push:
		push(reg_a);
		instrAddrForward();
		break;
	case InstrSet::Pop:
		pop(reg_a);
		instrAddrForward();
		break;

	case InstrSet::Load:
		instrAddrForward();
		fetch();
		reg_a->read32();
		instrAddrForward();
		break;
	case InstrSet::LoadRel:
		instrAddrForward();
		fetch();
		reg_mem->read_write32();
		fetch();
		reg_a->read32();
		instrAddrForward();
		break;
	case InstrSet::Store:
		instrAddrForward();
		fetch();
		reg_mem->read_write32();
		reg_a->write32();
		ram->read32();
		instrAddrForward();
		break;

	case InstrSet::Out:
		reg_a->write32();
		out->process();
		instrAddrForward();
		break;

	case InstrSet::Add:
		processALU(reg_0, reg_1, ALUInstrSet::Add32, reg_0);
		instrAddrForward();
		break;
	case InstrSet::Sub:
		processALU(reg_a, reg_b, ALUInstrSet::Sub32, reg_a);
		instrAddrForward();
		break;
	case InstrSet::Mul:
		processALU(reg_a, reg_b, ALUInstrSet::Mul32, reg_a);
		instrAddrForward();
		break;
	case InstrSet::Div:
		processALU(reg_a, reg_b, ALUInstrSet::Div32, reg_a);
		instrAddrForward();
		break;
	case InstrSet::Mod:
		processALU(reg_a, reg_b, ALUInstrSet::Mod32, reg_a);
		instrAddrForward();
		break;
	case InstrSet::LShift:
		processALU(reg_a, reg_b, ALUInstrSet::VLShift32, reg_a);
		instrAddrForward();
		break;
	case InstrSet::RShift:
		processALU(reg_a, reg_b, ALUInstrSet::VRShift32, reg_a);
		instrAddrForward();
		break;
	case InstrSet::Comp:
		processALU(reg_a, reg_b, ALUInstrSet::Comp32);
		instrAddrForward();
		break;

	case InstrSet::Inc:
		processALU(reg_a, ALUInstrSet::Inc32, reg_a);
		instrAddrForward();
		break;
	case InstrSet::Dec:
		processALU(reg_a, ALUInstrSet::Dec32, reg_a);
		instrAddrForward();
		break;
	case InstrSet::Neg:
		processALU(reg_a, ALUInstrSet::Neg32, reg_a);
		instrAddrForward();
		break;
	case InstrSet::Comp0:
		processALU(reg_a, ALUInstrSet::Comp032);
		instrAddrForward();
		break;

	case InstrSet::Halt:
		break;

	default: 
		std::cout << "Oups (unknown code: " << (int)bus_instr->extract_h() << ")"<< std::endl;
		std::cin.get();
		exit(1);
	}
}

void CU::prepareALU(MemoryControlFlow32* a, MemoryControlFlow32* b, InstrSetTertiary part) {
	switch(part) {
	case InstrSetTertiary::x:
		b->write32();
		reg_alu_tmp->read_write32();
		a->write32();
		break;

	case InstrSetTertiary::h:
		b->write16h();
		reg_alu_tmp->read_write32();
		a->write32();
		break;

	case InstrSetTertiary::l:
		b->write16l();
		reg_alu_tmp->read_write32();
		a->write32();
		break;

	case InstrSetTertiary::hh:
		b->write8hh();
		reg_alu_tmp->read_write32();
		a->write32();
		break;

	case InstrSetTertiary::hl:
		b->write8hl();
		reg_alu_tmp->read_write32();
		a->write32();
		break;

	case InstrSetTertiary::lh:
		b->write8hl();
		reg_alu_tmp->read_write32();
		a->write32();
		break;

	case InstrSetTertiary::ll:
		b->write8ll();
		reg_alu_tmp->read_write8ll();
		a->write32();
		break;


	}

	b->write32();
	reg_alu_tmp->read_write32();
	a->write32();
}

void CU::prepareALU(MemoryControlFlow32* a) {
	a->write32();
}

void CU::callALU(ALUInstrSet instr) {
	bus_alu->bind(bytev(instr));
	alu->process();
	reg_flag->read_write8();
}

void CU::ALURes_to(MemoryControlFlow32* a) {
	reg_alu_res->read_write32();
	a->read32();
}

void CU::instrAddrForward() {
	reg_instr_addr->write32();
	bus_alu->bind(bytev(ALUInstrSet::Inc32));
	alu->process();
	reg_alu_res->read_write32();
	bus_alu->bind(bytev(ALUInstrSet::Inc32));
	alu->process();
	reg_alu_res->read_write32();
	bus_alu->bind(bytev(ALUInstrSet::Inc32));
	alu->process();
	reg_alu_res->read_write32();
	bus_alu->bind(bytev(ALUInstrSet::Inc32));
	alu->process();
	reg_alu_res->read_write32();
	reg_instr_addr->read32();
}

void CU::fetch() {
	reg_instr_addr->write32();
	reg_mem->read_write32();
	ram->write32();
	reg_instr->read_write32();
}

void CU::push(MemoryControlFlow32* reg) {
	// write to addr bus
	reg_seg_stack->write32();
	reg_mem->read_write32();
	// inc stack_ptr
	bus_alu->bind(bytev(ALUInstrSet::Inc32));
	alu->process();
	reg_alu_res->read_write32();
	reg_seg_stack->read32();
	// write to ram
	reg->write32();
	ram->read32();
}

void CU::pop(MemoryControlFlow32* reg) {
	// dec stack_ptr
	reg_seg_stack->write32();
	bus_alu->bind(bytev(ALUInstrSet::Dec32));
	alu->process();
	reg_alu_res->read_write32();
	reg_seg_stack->read32();
	// write to addr bus
	reg_mem->read_write32();
	// read from ram
	ram->write32();
	reg->read32();
}

void CU::jmp_if(bool value) {
	if(value) {
		fetch();
		reg_instr_addr->read32();
	} else {
		instrAddrForward();
	}
}

bool CU::is_flag(ALUFlag f) {
	return (bus_flag->extract() & bytev(f)) != 0;
}

bool CU::is_not_flag(ALUFlag f) {
	return !is_flag(f);
}

END_NS