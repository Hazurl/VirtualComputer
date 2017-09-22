#pragma once

#include "config.h"
#include "Bus.h"
#include "Register.h"
#include "ALU.h"
#include "Processable.h"
#include "MemoryControlFlow.h"
#include "InstrSet.h"

BEGIN_NS

class CU : public Processable {
public:
	CU(MemoryControlFlow* ram, Processable* alu, Processable* out, MemoryControlFlow* reg_mem, MemoryControlFlow* reg_flag, 
	   Bus* bus_flag, MemoryControlFlow* reg_instr, Bus* bus_instr, MemoryControlFlow* reg_instr_addr, Bus* bus_alu, MemoryControlFlow* reg_alu_res,
	   MemoryControlFlow* reg_alu_tmp, MemoryControlFlow* reg_a, MemoryControlFlow* reg_b, MemoryControlFlow* reg_c,
	   MemoryControlFlow* reg_seg_instr, MemoryControlFlow* reg_seg_stack, MemoryControlFlow* reg_stack_ptr);

	void process() override;

private:

	void prepareALU(MemoryControlFlow* a, MemoryControlFlow* b);
	void prepareALU(MemoryControlFlow* a);
	void callALU(ALUInstrSet instr);
	void ALURes_to(MemoryControlFlow* a);

	inline void processALU(MemoryControlFlow* a, MemoryControlFlow* b, ALUInstrSet instr, MemoryControlFlow* res) {
		prepareALU(a, b);
		callALU(instr);
		ALURes_to(a);
	}

	inline void processALU(MemoryControlFlow* a, ALUInstrSet instr, MemoryControlFlow* res) {
		prepareALU(a);
		callALU(instr);
		ALURes_to(a);
	}

	inline void processALU(MemoryControlFlow* a, MemoryControlFlow* b, ALUInstrSet instr) { 
		prepareALU(a, b);
		callALU(instr);
	}

	inline void processALU(MemoryControlFlow* a, ALUInstrSet instr) { 
		prepareALU(a);
		callALU(instr);
	}

	void instrAddrForward();
	void fetch();
	void jmp_if(bool value);
	bool is_flag(ALUFlag f);
	bool is_not_flag(ALUFlag f);

	void push(MemoryControlFlow* reg);
	void pop(MemoryControlFlow* reg);

	Processable* alu;
	Processable* out;

	MemoryControlFlow* ram;
	MemoryControlFlow* reg_mem;

	MemoryControlFlow* reg_flag;
	Bus* bus_flag;
	
	MemoryControlFlow* reg_instr;
	Bus* bus_instr;
	MemoryControlFlow* reg_instr_addr;

	Bus* bus_alu;
	MemoryControlFlow* reg_alu_res;
	MemoryControlFlow* reg_alu_tmp;

	MemoryControlFlow* reg_a;
	MemoryControlFlow* reg_b;
	MemoryControlFlow* reg_c;

	MemoryControlFlow* reg_seg_instr;
	MemoryControlFlow* reg_seg_stack;
	MemoryControlFlow* reg_stack_ptr;
};

END_NS