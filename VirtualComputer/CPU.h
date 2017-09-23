#pragma once

#include "ALU.h"
#include "Bus.h"
#include "CU.h"
#include "Register.h"

BEGIN_NS

class CPU {
public:
	CPU(Bus16* bus_data, Bus16* bus_addr, MemoryControlFlow* ram, Processable* out);

	void define_program(ubyte entry_point, ubyte segment_stack);
	void start_program();

private:

	Bus16* bus_data;
	Bus16* bus_addr;

	Bus16 bus_instr;
	Bus16 bus_ALURes;
	Bus16 bus_ALUTmp;
	Bus16 bus_ALUCode;
	Bus16 bus_ALUFlag;
	Bus16 bus_CUFlag;

	Register8 reg_memory;
	Register8 reg_a;
	Register8 reg_b;
	Register8 reg_c;
	Register8 reg_instr;
	Register8 reg_instrAddr;
	Register8 reg_ALUTmp;
	Register8 reg_ALURes;
	Register8 reg_flag;
	Register8 reg_stack_ptr;

	Register8 reg_seg_instr;
	Register8 reg_seg_stack;

	ALU alu;
	CU cu;
};

END_NS