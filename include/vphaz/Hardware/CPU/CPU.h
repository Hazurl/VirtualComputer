#pragma once

#include <vphaz/Hardware/CPU/ALU.h>
#include <vphaz/Hardware/Bus.h>
#include <vphaz/Hardware/CPU/CU.h>
#include <vphaz/Hardware/Memory/Register.h>

BEGIN_NS

class CPU {
public:
	CPU(Bus32* bus_data, Bus32* bus_addr, MemoryControlFlow32* ram, Processable* out);

	void define_program(udword segment_instr, udword segment_data, udword segment_bss, udword segment_stack, udword entry_point);
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
	Register32 reg_seg_data;
	Register32 reg_seg_bss;

	ALU alu;
	CU cu;
};

END_NS