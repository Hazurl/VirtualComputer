#include "stdafx.h"
#include "CU.h"

BEGIN_NS

CU::CU(Processable* alu, MemoryControlFlow * flag_reg, Bus * flag_bus, MemoryControlFlow * instr_reg, Bus * instr_bus, MemoryControlFlow * instrAddr_reg, Bus * ALU_bus, MemoryControlFlow * ALUres_reg, 
	   MemoryControlFlow * ALUtmp_reg, MemoryControlFlow * a_reg, MemoryControlFlow * b_reg, MemoryControlFlow * c_reg)
  : alu(alu),
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
	c_reg(c_reg) 
{

}

void CU::process() {
	switch(static_cast<CU::OP>(instr_bus->extract())) {
	case OP::ADD:
		prepareALU_regab(ALU::CU_CODE::ADD);
		break;

	case OP::SUB:
		prepareALU_regab(ALU::CU_CODE::SUB);
		break;

	case OP::MUL:
		prepareALU_regab(ALU::CU_CODE::MUL);
		break;

	case OP::DIV:
		prepareALU_regab(ALU::CU_CODE::DIV);
		break;

	case OP::MOD:
		prepareALU_regab(ALU::CU_CODE::MOD);
		break;

	case OP::VLSHFT:
		prepareALU_regab(ALU::CU_CODE::VLSHFT);
		break;

	case OP::VRSHFT:
		prepareALU_regab(ALU::CU_CODE::VRSHFT);
		break;

	case OP::COMP:
		prepareALU_regab(ALU::CU_CODE::COMP);
		break;

	case OP::NEG:
		prepareALU_rega(ALU::CU_CODE::NEG);
		break;

	case OP::INC:
		prepareALU_rega(ALU::CU_CODE::INC);
		break;

	case OP::DEC:
		prepareALU_rega(ALU::CU_CODE::DEC);
		break;

	case OP::RSHFT:
		prepareALU_rega(ALU::CU_CODE::RSHFT);
		break;

	case OP::LSHFT:
		prepareALU_rega(ALU::CU_CODE::LSHFT);
		break;

	case OP::DECCOUNTER:
		prepareALU_regc(ALU::CU_CODE::DEC);
		break;

	case OP::INCCOUNTER:
		prepareALU_regc(ALU::CU_CODE::INC);
		break;

	case OP::LOADA:
		a_reg->read();
		break;

	case OP::LOADB:
		b_reg->read();
		break;

	case OP::LOADC:
		c_reg->read();
		break;

	case OP::OUTA:
		a_reg->write();
		break;

	case OP::OUTB:
		b_reg->write();
		break;

	case OP::OUTC:
		c_reg->write();
		break;

	default: 
		exit(1);
	}
}


void CU::prepareALU_regab(ALU::CU_CODE code) {
	b_reg->write();
	ALUtmp_reg->read();
	ALUtmp_reg->write();
	a_reg->write();
	ALU_bus->bind(static_cast<byte>(code));

	alu->process();

	ALUres_reg->read();
	ALUres_reg->write();
	a_reg->read();
}

void CU::prepareALU_regc(ALU::CU_CODE code) {
	c_reg->write();
	ALU_bus->bind(static_cast<byte>(code));

	alu->process();

	ALUres_reg->read();
	ALUres_reg->write();
	c_reg->read();
}

void CU::prepareALU_rega(ALU::CU_CODE code) {
	a_reg->write();
	ALU_bus->bind(static_cast<byte>(code));

	alu->process();

	ALUres_reg->read();
	ALUres_reg->write();
	a_reg->read();
}
END_NS