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
	
	switch(static_cast<InstrSet>(bus_instr->extract())) {
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

	case InstrSet::Add_ab:
		processALU(reg_a, reg_b, ALUInstrSet::Add32, reg_a);
		instrAddrForward();
		break;
	case InstrSet::Sub_ab:
		processALU(reg_a, reg_b, ALUInstrSet::Sub32, reg_a);
		instrAddrForward();
		break;
	case InstrSet::Mul_ab:
		processALU(reg_a, reg_b, ALUInstrSet::Mul32, reg_a);
		instrAddrForward();
		break;
	case InstrSet::Div_ab:
		processALU(reg_a, reg_b, ALUInstrSet::Div32, reg_a);
		instrAddrForward();
		break;
	case InstrSet::Mod_ab:
		processALU(reg_a, reg_b, ALUInstrSet::Mod32, reg_a);
		instrAddrForward();
		break;
	case InstrSet::LShift_ab:
		processALU(reg_a, reg_b, ALUInstrSet::VLShift32, reg_a);
		instrAddrForward();
		break;
	case InstrSet::RShift_ab:
		processALU(reg_a, reg_b, ALUInstrSet::VRShift32, reg_a);
		instrAddrForward();
		break;
	case InstrSet::Comp_ab:
		processALU(reg_a, reg_b, ALUInstrSet::Comp32);
		instrAddrForward();
		break;
	case InstrSet::Move_ab:
		reg_a->write32();
		reg_b->read32();
		instrAddrForward();
		break;
	case InstrSet::Move_ba:
		reg_b->write32();
		reg_a->read32();
		instrAddrForward();
		break;

	case InstrSet::Inc_a:
		processALU(reg_a, ALUInstrSet::Inc32, reg_a);
		instrAddrForward();
		break;
	case InstrSet::Dec_a:
		processALU(reg_a, ALUInstrSet::Dec32, reg_a);
		instrAddrForward();
		break;
	case InstrSet::Neg_a:
		processALU(reg_a, ALUInstrSet::Neg32, reg_a);
		instrAddrForward();
		break;
	case InstrSet::LShift_a:
		processALU(reg_a, ALUInstrSet::LShift32, reg_a);
		instrAddrForward();
		break;
	case InstrSet::RShift_a:
		processALU(reg_a, ALUInstrSet::RShift32, reg_a);
		instrAddrForward();
		break;
	case InstrSet::Comp0_a:
		processALU(reg_a, ALUInstrSet::Comp032);
		instrAddrForward();
		break;
	case InstrSet::Push_a:
		push(reg_a);
		instrAddrForward();
		break;
	case InstrSet::Pop_a:
		pop(reg_a);
		instrAddrForward();
		break;
	case InstrSet::Load_a:
		instrAddrForward();
		fetch();
		reg_a->read32();
		instrAddrForward();
		break;
	case InstrSet::LoadRel_a:
		instrAddrForward();
		fetch();
		reg_mem->read_write32();
		fetch();
		reg_a->read32();
		instrAddrForward();
		break;
	case InstrSet::Store_a:
		instrAddrForward();
		fetch();
		reg_mem->read_write32();
		reg_a->write32();
		ram->read32();
		instrAddrForward();
		break;
	case InstrSet::Out_a:
		reg_a->write32();
		out->process();
		instrAddrForward();
		break;

	case InstrSet::Inc_b:
		processALU(reg_b, ALUInstrSet::Inc32, reg_b);
		instrAddrForward();
		break;
	case InstrSet::Dec_b:
		processALU(reg_b, ALUInstrSet::Dec32, reg_b);
		instrAddrForward();
		break;
	case InstrSet::Neg_b:
		processALU(reg_b, ALUInstrSet::Neg32, reg_b);
		instrAddrForward();
		break;
	case InstrSet::LShift_b:
		processALU(reg_b, ALUInstrSet::LShift32, reg_b);
		instrAddrForward();
		break;
	case InstrSet::RShift_b:
		processALU(reg_b, ALUInstrSet::RShift32, reg_b);
		instrAddrForward();
		break;
	case InstrSet::Comp0_b:
		processALU(reg_b, ALUInstrSet::Comp032);
		instrAddrForward();
		break;
	case InstrSet::Push_b:
		push(reg_b);
		instrAddrForward();
		break;
	case InstrSet::Pop_b:
		pop(reg_b);
		instrAddrForward();
		break;
	case InstrSet::Load_b:
		instrAddrForward();
		fetch();
		reg_b->read32();
		instrAddrForward();
		break;
	case InstrSet::LoadRel_b:
		instrAddrForward();
		fetch();
		reg_mem->read_write32();
		fetch();
		reg_b->read32();
		instrAddrForward();
		break;
	case InstrSet::Store_b:
		instrAddrForward();
		fetch();
		reg_mem->read_write32();
		reg_b->write32();
		ram->read32();
		instrAddrForward();
		break;
	case InstrSet::Out_b:
		reg_b->write32();
		out->process();
		instrAddrForward();
		break;

	case InstrSet::Inc_c:
		processALU(reg_c, ALUInstrSet::Inc32, reg_c);
		instrAddrForward();
		break;
	case InstrSet::Dec_c:
		processALU(reg_c, ALUInstrSet::Dec32, reg_c);
		instrAddrForward();
		break;
	case InstrSet::Neg_c:
		processALU(reg_c, ALUInstrSet::Neg32, reg_c);
		instrAddrForward();
		break;
	case InstrSet::LShift_c:
		processALU(reg_c, ALUInstrSet::LShift32, reg_c);
		instrAddrForward();
		break;
	case InstrSet::RShift_c:
		processALU(reg_c, ALUInstrSet::RShift32, reg_c);
		instrAddrForward();
		break;
	case InstrSet::Comp0_c:
		processALU(reg_c, ALUInstrSet::Comp032);
		instrAddrForward();
		break;
	case InstrSet::Push_c:
		push(reg_c);
		instrAddrForward();
		break;
	case InstrSet::Pop_c:
		pop(reg_c);
		instrAddrForward();
		break;
	case InstrSet::Load_c:
		instrAddrForward();
		fetch();
		reg_c->read32();
		instrAddrForward();
		break;
	case InstrSet::LoadRel_c:
		instrAddrForward();
		fetch();
		reg_mem->read_write32();
		fetch();
		reg_c->read32();
		instrAddrForward();
		break;
	case InstrSet::Store_c:
		instrAddrForward();
		fetch();
		reg_mem->read_write32();
		reg_c->write32();
		ram->read32();
		instrAddrForward();
		break;
	case InstrSet::Out_c:
		reg_c->write32();
		out->process();
		instrAddrForward();
		break;

	case InstrSet::Halt:
		break;

	default: 
		std::cout << "Oups (unknown code: " << (int)bus_instr->extract() << ")"<< std::endl;
		std::cin.get();
		exit(1);
	}
}

void CU::prepareALU(MemoryControlFlow32* a, MemoryControlFlow32* b) {
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