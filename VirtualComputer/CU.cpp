#include "stdafx.h"
#include "CU.h"
#include <iostream>

BEGIN_NS

CU::CU(MemoryControlFlow* ram, Processable* alu, Processable* out, MemoryControlFlow* reg_mem, MemoryControlFlow * reg_flag,
	   Bus * bus_flag, MemoryControlFlow * reg_instr, Bus * bus_instr, MemoryControlFlow * reg_instr_addr, Bus * bus_alu, MemoryControlFlow * reg_alu_res,
	   MemoryControlFlow * reg_alu_tmp, MemoryControlFlow * reg_a, MemoryControlFlow * reg_b, MemoryControlFlow * reg_c,
	   MemoryControlFlow* reg_seg_instr, MemoryControlFlow* reg_seg_stack, MemoryControlFlow* reg_stack_ptr)
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
		processALU(reg_a, reg_b, ALUInstrSet::Add, reg_a);
		instrAddrForward();
		break;
	case InstrSet::Sub_ab:
		processALU(reg_a, reg_b, ALUInstrSet::Sub, reg_a);
		instrAddrForward();
		break;
	case InstrSet::Mul_ab:
		processALU(reg_a, reg_b, ALUInstrSet::Mul, reg_a);
		instrAddrForward();
		break;
	case InstrSet::Div_ab:
		processALU(reg_a, reg_b, ALUInstrSet::Div, reg_a);
		instrAddrForward();
		break;
	case InstrSet::Mod_ab:
		processALU(reg_a, reg_b, ALUInstrSet::Mod, reg_a);
		instrAddrForward();
		break;
	case InstrSet::LShift_ab:
		processALU(reg_a, reg_b, ALUInstrSet::VLShift, reg_a);
		instrAddrForward();
		break;
	case InstrSet::RShift_ab:
		processALU(reg_a, reg_b, ALUInstrSet::VRShift, reg_a);
		instrAddrForward();
		break;
	case InstrSet::Comp_ab:
		processALU(reg_a, reg_b, ALUInstrSet::Comp);
		instrAddrForward();
		break;
	case InstrSet::Move_ab:
		reg_a->write();
		reg_b->read();
		instrAddrForward();
		break;
	case InstrSet::Move_ba:
		reg_b->write();
		reg_a->read();
		instrAddrForward();
		break;

	case InstrSet::Inc_a:
		processALU(reg_a, ALUInstrSet::Inc, reg_a);
		instrAddrForward();
		break;
	case InstrSet::Dec_a:
		processALU(reg_a, ALUInstrSet::Dec, reg_a);
		instrAddrForward();
		break;
	case InstrSet::Neg_a:
		processALU(reg_a, ALUInstrSet::Neg, reg_a);
		instrAddrForward();
		break;
	case InstrSet::LShift_a:
		processALU(reg_a, ALUInstrSet::LShift, reg_a);
		instrAddrForward();
		break;
	case InstrSet::RShift_a:
		processALU(reg_a, ALUInstrSet::RShift, reg_a);
		instrAddrForward();
		break;
	case InstrSet::Comp0_a:
		processALU(reg_a, ALUInstrSet::Comp0);
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
		reg_a->read();
		instrAddrForward();
		break;
	case InstrSet::LoadRel_a:
		instrAddrForward();
		fetch();
		reg_mem->read_write();
		fetch();
		reg_a->read();
		instrAddrForward();
		break;
	case InstrSet::Store_a:
		instrAddrForward();
		fetch();
		reg_mem->read_write();
		reg_a->write();
		ram->read();
		instrAddrForward();
		break;
	case InstrSet::Out_a:
		reg_a->write();
		out->process();
		instrAddrForward();
		break;

	case InstrSet::Inc_b:
		processALU(reg_b, ALUInstrSet::Inc, reg_b);
		instrAddrForward();
		break;
	case InstrSet::Dec_b:
		processALU(reg_b, ALUInstrSet::Dec, reg_b);
		instrAddrForward();
		break;
	case InstrSet::Neg_b:
		processALU(reg_b, ALUInstrSet::Neg, reg_b);
		instrAddrForward();
		break;
	case InstrSet::LShift_b:
		processALU(reg_b, ALUInstrSet::LShift, reg_b);
		instrAddrForward();
		break;
	case InstrSet::RShift_b:
		processALU(reg_b, ALUInstrSet::RShift, reg_b);
		instrAddrForward();
		break;
	case InstrSet::Comp0_b:
		processALU(reg_b, ALUInstrSet::Comp0);
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
		reg_b->read();
		instrAddrForward();
		break;
	case InstrSet::LoadRel_b:
		instrAddrForward();
		fetch();
		reg_mem->read_write();
		fetch();
		reg_b->read();
		instrAddrForward();
		break;
	case InstrSet::Store_b:
		instrAddrForward();
		fetch();
		reg_mem->read_write();
		reg_b->write();
		ram->read();
		instrAddrForward();
		break;
	case InstrSet::Out_b:
		reg_b->write();
		out->process();
		instrAddrForward();
		break;

	case InstrSet::Inc_c:
		processALU(reg_c, ALUInstrSet::Inc, reg_c);
		instrAddrForward();
		break;
	case InstrSet::Dec_c:
		processALU(reg_c, ALUInstrSet::Dec, reg_c);
		instrAddrForward();
		break;
	case InstrSet::Neg_c:
		processALU(reg_c, ALUInstrSet::Neg, reg_c);
		instrAddrForward();
		break;
	case InstrSet::LShift_c:
		processALU(reg_c, ALUInstrSet::LShift, reg_c);
		instrAddrForward();
		break;
	case InstrSet::RShift_c:
		processALU(reg_c, ALUInstrSet::RShift, reg_c);
		instrAddrForward();
		break;
	case InstrSet::Comp0_c:
		processALU(reg_c, ALUInstrSet::Comp0);
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
		reg_c->read();
		instrAddrForward();
		break;
	case InstrSet::LoadRel_c:
		instrAddrForward();
		fetch();
		reg_mem->read_write();
		fetch();
		reg_c->read();
		instrAddrForward();
		break;
	case InstrSet::Store_c:
		instrAddrForward();
		fetch();
		reg_mem->read_write();
		reg_c->write();
		ram->read();
		instrAddrForward();
		break;
	case InstrSet::Out_c:
		reg_c->write();
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

void CU::prepareALU(MemoryControlFlow* a, MemoryControlFlow* b) {
	b->write();
	reg_alu_tmp->read_write();
	a->write();
}

void CU::prepareALU(MemoryControlFlow* a) {
	a->write();
}

void CU::callALU(ALUInstrSet instr) {
	bus_alu->bind(bytev(instr));
	alu->process();
	reg_flag->read_write();
}

void CU::ALURes_to(MemoryControlFlow* a) {
	reg_alu_res->read_write();
	a->read();
}

void CU::instrAddrForward() {
	reg_instr_addr->write();
	bus_alu->bind(bytev(ALUInstrSet::Inc));
	alu->process();
	reg_alu_res->read_write();
	reg_instr_addr->read();
}

void CU::fetch() {
	reg_instr_addr->write();
	reg_mem->read_write();
	ram->write();
	reg_instr->read_write();
}

void CU::push(MemoryControlFlow* reg) {
	// write to addr bus
	reg_seg_stack->write();
	reg_mem->read_write();
	// inc stack_ptr
	bus_alu->bind(bytev(ALUInstrSet::Inc));
	alu->process();
	reg_alu_res->read_write();
	reg_seg_stack->read();
	// write to ram
	reg->write();
	ram->read();
}

void CU::pop(MemoryControlFlow* reg) {
	// dec stack_ptr
	reg_seg_stack->write();
	bus_alu->bind(bytev(ALUInstrSet::Dec));
	alu->process();
	reg_alu_res->read_write();
	reg_seg_stack->read();
	// write to addr bus
	reg_mem->read_write();
	// read from ram
	ram->write();
	reg->read();
}

void CU::jmp_if(bool value) {
	if(value) {
		fetch();
		reg_instr_addr->read();
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