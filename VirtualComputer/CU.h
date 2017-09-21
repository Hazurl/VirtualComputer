#pragma once

#include "config.h"
#include "Bus.h"
#include "Register.h"
#include "ALU.h"
#include "Processable.h"
#include "MemoryControlFlow.h"

BEGIN_NS

class CU : public Processable {
public:
	CU(MemoryControlFlow* ram, Processable* alu, Processable* out, MemoryControlFlow* mem_reg, MemoryControlFlow* flag_reg, 
	   Bus* flag_bus, MemoryControlFlow* instr_reg, Bus* instr_bus, MemoryControlFlow* instrAddr_reg, Bus* ALU_bus, MemoryControlFlow* ALUres_reg,
	   MemoryControlFlow* ALUtmp_reg, MemoryControlFlow* a_reg, MemoryControlFlow* b_reg, MemoryControlFlow* c_reg);

	enum class OP : byte {
		END = byte_min,
		ADD, SUB,
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

	void process() override;

private:

	void prepareALU_regab(ALU::CU_CODE code);
	void prepareALU_rega(ALU::CU_CODE code);
	void prepareALU_regc(ALU::CU_CODE code);

	void advanceInstrAddr();
	void loadInstr();
	void fetchFromRam();

	Processable* alu;
	Processable* out;

	MemoryControlFlow* ram;
	MemoryControlFlow* mem_reg;

	MemoryControlFlow* flag_reg;
	Bus* flag_bus;
	
	MemoryControlFlow* instr_reg;
	Bus* instr_bus;
	MemoryControlFlow* instrAddr_reg;

	Bus* ALU_bus;
	MemoryControlFlow* ALUres_reg;
	MemoryControlFlow* ALUtmp_reg;

	MemoryControlFlow* a_reg;
	MemoryControlFlow* b_reg;
	MemoryControlFlow* c_reg;
};

END_NS