#pragma once

#include "ALU.h"
#include "Bus.h"
#include "CU.h"
#include "Register.h"

BEGIN_NS

class CPU {
public:
	CPU(Bus* bus_data, Bus* bus_addr, MemoryControlFlow* ram, Processable* out);

	void define_program(ubyte entry_point, ubyte segment_stack);
	void start_program();

private:

	Bus* bus_data;
	Bus* bus_addr;

	Bus bus_instr;
	Bus bus_ALURes;
	Bus bus_ALUTmp;
	Bus bus_ALUCode;
	Bus bus_ALUFlag;
	Bus bus_CUFlag;

	Register reg_memory;
	Register reg_a;
	Register reg_b;
	Register reg_c;
	Register reg_instr;
	Register reg_instrAddr;
	Register reg_ALUTmp;
	Register reg_ALURes;
	Register reg_flag;
	Register reg_stack_ptr;

	Register reg_seg_instr;
	Register reg_seg_stack;

	ALU alu;
	CU cu;
};

END_NS