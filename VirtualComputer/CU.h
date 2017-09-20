#pragma once

#include "config.h"
#include "Bus.h"
#include "Register.h"
#include "ALU.h"

BEGIN_NS

class CU {
public:
	CU(ALU* alu, Register* flag_reg, Bus* flag_bus, Register* instr_reg, Bus* instr_bus, Register* instrAddr_reg, Bus* ALU_bus, Register* ALUres_reg, 
	   Register* ALUtmp_reg, Register* a_reg, Register* b_reg, Register* c_reg);

	enum class OP : byte {
		ADD = byte_min, SUB,
		MUL, DIV,
		NEG, MOD,
		INC, DEC,
		INCCOUNTER, DECCOUNTER,
		LSHFT, RSHFT,
		VLSHFT, VRSHFT,
		COMP,

		LOADA, STOREA,
		LOADB, STOREB,
		LOADC, STOREC,
		JMPEQ, JMPNE, JMPL, JMPG, JMPGE, JMPLE, JMP,

		OUTA, // std::cout, debug !
		OUTB, // std::cout, debug !
		OUTC, // std::cout, debug !
	};

	void process();

private:

	void prepareALU_regab(ALU::CU_CODE code);
	void prepareALU_rega(ALU::CU_CODE code);
	void prepareALU_regc(ALU::CU_CODE code);

	ALU* alu;

	Register* flag_reg;
	Bus* flag_bus;
	
	Register* instr_reg;
	Bus* instr_bus;
	Register* instrAddr_reg;

	Bus* ALU_bus;
	Register* ALUres_reg;
	Register* ALUtmp_reg;

	Register* a_reg;
	Register* b_reg;
	Register* c_reg;
};

END_NS