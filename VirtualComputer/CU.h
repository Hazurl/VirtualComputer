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
	   MemoryControlFlow* ALUtmp_reg, MemoryControlFlow* a_reg, MemoryControlFlow* b_reg, MemoryControlFlow* c_reg,
	   MemoryControlFlow* seg_instr_reg, MemoryControlFlow* seg_stack_reg, MemoryControlFlow* stack_ptr_reg);

	enum class OP : byte {
		END = byte_min,
		ADD, SUB,
		MUL, DIV,
		NEG, MOD,
		INC, DEC,
		INCCOUNTER, DECCOUNTER,
		LSHFT, RSHFT,
		VLSHFT, VRSHFT,
		COMP, COMPC0,

		LOADA, STOREA,
		LOADB, STOREB,
		LOADC, STOREC,
		PUSHA, POPA,
		PUSHB, POPB,
		PUSHC, POPC,
		JMPEQ, JMPNE, JMPL, JMPG, JMPGE, JMPLE, JMP, JMPC0,
		OUTA,
		OUTB,
		OUTC,
	};

	void process() override;

private:

	void prepareALU_regab(ALU::CU_CODE code, bool res_to_a = true);
	void prepareALU_rega(ALU::CU_CODE code, bool res_to_a = true);
	void prepareALU_regc(ALU::CU_CODE code, bool res_to_c = true);

	void advanceInstrAddr();
	void loadInstr();
	void fetchFromRam();
	void jmp_if(bool value);
	bool is_flag(ALU::FLAG_BIT f);
	bool is_not_flag(ALU::FLAG_BIT f);

	void push(MemoryControlFlow* reg);
	void pop(MemoryControlFlow* reg);

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

	MemoryControlFlow* seg_instr_reg;
	MemoryControlFlow* seg_stack_reg;
	MemoryControlFlow* stack_ptr_reg;
};

END_NS