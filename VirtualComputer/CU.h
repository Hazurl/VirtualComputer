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
	CU(MemoryControlFlow32* ram, Processable* alu, Processable* out, MemoryControlFlow32* reg_mem, MemoryControlFlow8* reg_flag, Bus8* bus_flag,
	   MemoryControlFlow32* reg_instr, Bus32* bus_instr, MemoryControlFlow32* reg_instr_addr, Bus8* bus_alu, MemoryControlFlow32* reg_alu_res,
	   MemoryControlFlow32* reg_alu_tmp, MemoryControlFlow32* reg_a, MemoryControlFlow32* reg_b, MemoryControlFlow32* reg_c,
	   MemoryControlFlow32* reg_seg_instr, MemoryControlFlow32* reg_seg_stack, MemoryControlFlow32* reg_stack_ptr);

	void process() override;

private:

	void prepareALU(MemoryControlFlow32* a, MemoryControlFlow32* b);
	void prepareALU(MemoryControlFlow32* a);
	void callALU(ALUInstrSet instr);
	void ALURes_to(MemoryControlFlow32* a);

	inline void processALU(MemoryControlFlow32* a, MemoryControlFlow32* b, ALUInstrSet instr, MemoryControlFlow32* res) {
		prepareALU(a, b);
		callALU(instr);
		ALURes_to(a);
	}

	inline void processALU(MemoryControlFlow32* a, ALUInstrSet instr, MemoryControlFlow32* res) {
		prepareALU(a);
		callALU(instr);
		ALURes_to(a);
	}

	inline void processALU(MemoryControlFlow32* a, MemoryControlFlow32* b, ALUInstrSet instr) {
		prepareALU(a, b);
		callALU(instr);
	}

	inline void processALU(MemoryControlFlow32* a, ALUInstrSet instr) {
		prepareALU(a);
		callALU(instr);
	}

	void instrAddrForward();
	void fetch();
	void jmp_if(bool value);
	bool is_flag(ALUFlag f);
	bool is_not_flag(ALUFlag f);

	void push(MemoryControlFlow32* reg);
	void pop(MemoryControlFlow32* reg);

	Processable* alu;
	Processable* out;

	MemoryControlFlow32* ram;
	MemoryControlFlow32* reg_mem;

	MemoryControlFlow8* reg_flag;
	Bus8* bus_flag;
	
	MemoryControlFlow32* reg_instr;
	Bus32* bus_instr;
	MemoryControlFlow32* reg_instr_addr;

	Bus8* bus_alu;
	MemoryControlFlow32* reg_alu_res;
	MemoryControlFlow32* reg_alu_tmp;

	MemoryControlFlow32* reg_a;
	MemoryControlFlow32* reg_b;
	MemoryControlFlow32* reg_c;

	MemoryControlFlow32* reg_seg_instr;
	MemoryControlFlow32* reg_seg_stack;
	MemoryControlFlow32* reg_stack_ptr;
};

END_NS