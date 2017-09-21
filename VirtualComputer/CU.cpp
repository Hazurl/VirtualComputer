#include "stdafx.h"
#include "CU.h"
#include <iostream>

BEGIN_NS

CU::CU(MemoryControlFlow* ram, Processable* alu, Processable* out, MemoryControlFlow* mem_reg, MemoryControlFlow * flag_reg, 
	   Bus * flag_bus, MemoryControlFlow * instr_reg, Bus * instr_bus, MemoryControlFlow * instrAddr_reg, Bus * ALU_bus, MemoryControlFlow * ALUres_reg,
	   MemoryControlFlow * ALUtmp_reg, MemoryControlFlow * a_reg, MemoryControlFlow * b_reg, MemoryControlFlow * c_reg,
	   MemoryControlFlow* seg_instr_reg, MemoryControlFlow* seg_stack_reg, MemoryControlFlow* stack_ptr_reg)
  : alu(alu),
	out(out),
	ram(ram),
	mem_reg(mem_reg),
	flag_reg(flag_reg),
	flag_bus(flag_bus),
	instr_reg(instr_reg),
	instr_bus(instr_bus),
	instrAddr_reg(instrAddr_reg),
	ALU_bus(ALU_bus),
	ALUres_reg(ALUres_reg),
	ALUtmp_reg(ALUtmp_reg),
	a_reg(a_reg),
	b_reg(b_reg),
	c_reg(c_reg),
	seg_instr_reg(seg_instr_reg),
	seg_stack_reg(seg_stack_reg),
	stack_ptr_reg(stack_ptr_reg)
{

}

void CU::process() {
	loadInstr();
	switch(static_cast<CU::OP>(instr_bus->extract())) {
	case OP::ADD:
		prepareALU_regab(ALU::CU_CODE::ADD);
		advanceInstrAddr();
		break;

	case OP::SUB:
		prepareALU_regab(ALU::CU_CODE::SUB);
		advanceInstrAddr();
		break;

	case OP::MUL:
		prepareALU_regab(ALU::CU_CODE::MUL);
		advanceInstrAddr();
		break;

	case OP::DIV:
		prepareALU_regab(ALU::CU_CODE::DIV);
		advanceInstrAddr();
		break;

	case OP::MOD:
		prepareALU_regab(ALU::CU_CODE::MOD);
		advanceInstrAddr();
		break;

	case OP::VLSHFT:
		prepareALU_regab(ALU::CU_CODE::VLSHFT);
		advanceInstrAddr();
		break;

	case OP::VRSHFT:
		prepareALU_regab(ALU::CU_CODE::VRSHFT);
		advanceInstrAddr();
		break;

	case OP::COMP:
		prepareALU_regab(ALU::CU_CODE::COMP, false);
		advanceInstrAddr();
		break;

	case OP::NEG:
		prepareALU_rega(ALU::CU_CODE::NEG);
		advanceInstrAddr();
		break;

	case OP::INC:
		prepareALU_rega(ALU::CU_CODE::INC);
		advanceInstrAddr();
		break;

	case OP::DEC:
		prepareALU_rega(ALU::CU_CODE::DEC);
		advanceInstrAddr();
		break;

	case OP::RSHFT:
		prepareALU_rega(ALU::CU_CODE::RSHFT);
		advanceInstrAddr();
		break;

	case OP::LSHFT:
		prepareALU_rega(ALU::CU_CODE::LSHFT);
		advanceInstrAddr();
		break;

	case OP::DECCOUNTER:
		prepareALU_regc(ALU::CU_CODE::DEC);
		advanceInstrAddr();
		break;

	case OP::INCCOUNTER:
		prepareALU_regc(ALU::CU_CODE::INC);
		advanceInstrAddr();
		break;

	case OP::COMPC0:
		prepareALU_regc(ALU::CU_CODE::COMP0, false);
		advanceInstrAddr();
		break;

	case OP::LOADA:
		advanceInstrAddr();
		fetchFromRam();
		a_reg->read();
		advanceInstrAddr();
		break;

	case OP::LOADB:
		advanceInstrAddr();
		fetchFromRam();
		b_reg->read();
		advanceInstrAddr();
		break;

	case OP::LOADC:
		advanceInstrAddr();
		fetchFromRam();
		c_reg->read();
		advanceInstrAddr();
		break;

	case OP::PUSHA:
		push(a_reg);
		advanceInstrAddr();
		break;

	case OP::PUSHB:
		push(b_reg);
		advanceInstrAddr();
		break;

	case OP::PUSHC:
		push(c_reg);
		advanceInstrAddr();
		break;

	case OP::POPA:
		pop(a_reg);
		advanceInstrAddr();
		break;

	case OP::POPB:
		pop(b_reg);
		advanceInstrAddr();
		break;

	case OP::POPC:
		pop(c_reg);
		advanceInstrAddr();
		break;

	case OP::OUTA:
		a_reg->write();
		out->process();
		advanceInstrAddr();
		break;

	case OP::OUTB:
		b_reg->write();
		out->process();
		advanceInstrAddr();
		break;

	case OP::OUTC:
		c_reg->write();
		out->process();
		advanceInstrAddr();
		break;

	case OP::JMP:
		advanceInstrAddr();
		fetchFromRam();
		instrAddr_reg->read();
		break;

	case OP::JMPEQ:
		advanceInstrAddr();
		jmp_if(is_flag(ALU::FLAG_BIT::Zero));
		break;

	case OP::JMPNE:
		advanceInstrAddr();
		jmp_if(is_not_flag(ALU::FLAG_BIT::Zero));
		break;

	case OP::JMPL:
		advanceInstrAddr();
		jmp_if(is_not_flag(ALU::FLAG_BIT::Comp) && is_not_flag(ALU::FLAG_BIT::Zero));
		break;

	case OP::JMPLE:
		advanceInstrAddr();
		jmp_if(is_flag(ALU::FLAG_BIT::Zero) || is_not_flag(ALU::FLAG_BIT::Comp));
		break;

	case OP::JMPG:
		advanceInstrAddr();
		jmp_if(is_flag(ALU::FLAG_BIT::Comp));
		break;

	case OP::JMPGE:
		advanceInstrAddr();
		jmp_if(is_flag(ALU::FLAG_BIT::Comp) || is_flag(ALU::FLAG_BIT::Zero));
		break;

	case OP::END:
		break;

	default: 
		std::cout << "Oups (unknown code: " << (int)instr_bus->extract() << ")"<< std::endl;
		std::cin.get();
		exit(1);
	}
}


void CU::prepareALU_regab(ALU::CU_CODE code, bool res_to_a) {
	b_reg->write();
	ALUtmp_reg->read_write();
	a_reg->write();
	ALU_bus->bind(static_cast<byte>(code));

	alu->process();
	flag_reg->read_write();

	ALUres_reg->read_write();
	if(res_to_a)
		a_reg->read();
}

void CU::prepareALU_regc(ALU::CU_CODE code, bool res_to_c) {
	c_reg->write();
	ALU_bus->bind(static_cast<byte>(code));

	alu->process();
	flag_reg->read_write();

	ALUres_reg->read_write();
	if(res_to_c)
		c_reg->read();
}

void CU::prepareALU_rega(ALU::CU_CODE code, bool res_to_a) {
	a_reg->write();
	ALU_bus->bind(static_cast<byte>(code));

	alu->process();
	flag_reg->read_write();

	ALUres_reg->read_write();
	if (res_to_a)
		a_reg->read();
}

void CU::advanceInstrAddr() {
	instrAddr_reg->write();
	ALU_bus->bind(static_cast<byte>(ALU::CU_CODE::INC));
	alu->process();
	ALUres_reg->read_write();
	instrAddr_reg->read();
}

void CU::fetchFromRam() {
	instrAddr_reg->write();
	mem_reg->read_write();
	ram->write();
}

void CU::loadInstr() {
	fetchFromRam();
	instr_reg->read_write();
}

void CU::push(MemoryControlFlow* reg) {
	// write to addr bus
	seg_stack_reg->write();
	mem_reg->read_write();
	// inc stack_ptr
	ALU_bus->bind(static_cast<byte>(ALU::CU_CODE::INC));
	alu->process();
	ALUres_reg->read_write();
	seg_stack_reg->read();
	// write to ram
	reg->write();
	ram->read();
}

void CU::pop(MemoryControlFlow* reg) {
	// dec stack_ptr
	seg_stack_reg->write();
	ALU_bus->bind(static_cast<byte>(ALU::CU_CODE::DEC));
	alu->process();
	ALUres_reg->read_write();
	seg_stack_reg->read();
	// write to addr bus
	mem_reg->read_write();
	// read from ram
	ram->write();
	reg->read();
}

void CU::jmp_if(bool value) {
	if(value) {
		fetchFromRam();
		instrAddr_reg->read();
	} else {
		advanceInstrAddr();
	}
}

bool CU::is_flag(ALU::FLAG_BIT f) {
	return (flag_bus->extract() & (1 << static_cast<byte>(f))) != 0;
}

bool CU::is_not_flag(ALU::FLAG_BIT f) {
	return !is_flag(f);
}

END_NS