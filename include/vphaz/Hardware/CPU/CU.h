#pragma once
#include <vphaz/config.h>

#include <vphaz/Hardware/Bus.h>
#include <vphaz/Hardware/Memory/MemoryControlFlow.h>
#include <vphaz/Hardware/Processable.h>
#include <vphaz/Hardware/CPU/InstrSet.h>
#include <vphaz/Hardware/CPU/ALU.h>
#include <vphaz/Exception/ALUInstrSetUnknown.h>
#include <vphaz/Exception/InstrSetUnknown.h>


BEGIN_NS

class CU : public Processable {
public:
	CU(MemoryControlFlow32* ram, Processable* alu, Processable* out, MemoryControlFlow32* reg_mem, MemoryControlFlow8* reg_flag, Bus8* bus_flag,
	   MemoryControlFlow32* reg_instr, Bus32* bus_instr, MemoryControlFlow32* reg_instr_addr, Bus8* bus_alu, MemoryControlFlow32* reg_alu_res,
	   MemoryControlFlow32* reg_alu_tmp, MemoryControlFlow32* reg_a, MemoryControlFlow32* reg_b, MemoryControlFlow32* reg_c,
	   MemoryControlFlow32* reg_seg_instr, MemoryControlFlow32* reg_seg_stack, MemoryControlFlow32* reg_stack_ptr);

	bool halted = false;
	void process() override;

private:

	void processALU(InstrTarget t0, InstrTarget t1, ALUInstrSet b8, ALUInstrSet b16, ALUInstrSet b32, bool result = true);
	void processALU(InstrTarget t0, ALUInstrSet b8, ALUInstrSet b16, ALUInstrSet b32, bool result = true);

	void is_defer(InstrTarget t0, InstrTarget t1);
	void is_comp(InstrTarget t0, InstrTarget t1);
	void is_comp0(InstrTarget t0, InstrTarget t1);
	void is_add(InstrTarget t0, InstrTarget t1);
	void is_sub(InstrTarget t0, InstrTarget t1);
	void is_mul(InstrTarget t0, InstrTarget t1);
	void is_div(InstrTarget t0, InstrTarget t1);
	void is_mod(InstrTarget t0, InstrTarget t1);
	void is_inc(InstrTarget t0, InstrTarget t1);
	void is_dec(InstrTarget t0, InstrTarget t1);
	void is_neg(InstrTarget t0, InstrTarget t1);
	void is_lshift(InstrTarget t0, InstrTarget t1);
	void is_rshift(InstrTarget t0, InstrTarget t1);
	void is_push(InstrTarget t0, InstrTarget t1);
	void is_pop(InstrTarget t0, InstrTarget t1);
	void is_move(InstrTarget t0, InstrTarget t1);
	void is_out(InstrTarget t0, InstrTarget t1);
	void is_jmp(InstrTarget t0, InstrTarget t1, bool value);

	bool is_flag(ALUFlag f);
	bool is_not_flag(ALUFlag f);

	bool is_reg(InstrTarget t);
	ubyte target_size(InstrTarget t);

	void target_read(InstrTarget t);
	void target_write(InstrTarget t);

	void instrAddrForward();
	void fetch();

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