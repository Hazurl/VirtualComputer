#pragma once

#include "ALU.h"
#include "Bus.h"
#include "CU.h"
#include "Register.h"

BEGIN_NS

class CPU {
public:
	CPU(Bus32* bus_data, Bus32* bus_addr, MemoryControlFlow32* ram, Processable* out);

	void define_program(ubyte entry_point, ubyte segment_stack);
	void start_program();

private:

	Bus32* bus_data;
	Bus32* bus_addr;

	Bus32 bus_instr;
	Bus32 bus_ALURes;
	Bus32 bus_ALUTmp;
	Bus8 bus_ALUCode;
	Bus8 bus_ALUflag;
	Bus8 bus_CUflag;

	Register8 reg_flag;
	Register32 reg_memory;
	Register32 reg_a;
	Register32 reg_b;
	Register32 reg_c;
	Register32 reg_instr;
	Register32 reg_instrAddr;
	Register32 reg_ALUTmp;
	Register32 reg_ALURes;
	Register32 reg_stack_ptr;

	Register32 reg_seg_instr;
	Register32 reg_seg_stack;

	ALU alu;
	CU cu;
};

END_NS