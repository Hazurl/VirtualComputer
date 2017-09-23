#include "stdafx.h"
#include "CPU.h"
#include <iostream>
BEGIN_NS

CPU::CPU(Bus32* bus_data, Bus32* bus_addr, MemoryControlFlow32* ram, Processable* out) :
	bus_data(bus_data),
	reg_flag(&bus_ALUflag, &bus_CUflag),
	reg_memory(bus_data, bus_addr),
	reg_a(bus_data, bus_data),
	reg_b(bus_data, bus_data),
	reg_c(bus_data, bus_data),
	reg_instr(bus_data, &bus_instr),
	reg_instrAddr(bus_data, bus_data),
	reg_ALUTmp(bus_data, &bus_ALUTmp),
	reg_ALURes(&bus_ALURes, bus_data),
	reg_stack_ptr(bus_data, bus_data),
	reg_seg_instr(bus_data, bus_data),
	reg_seg_stack(bus_data, bus_data),
	alu(&bus_ALUCode, &bus_ALUflag, &bus_ALURes, bus_data, &bus_ALUTmp),
	cu(ram, &alu, out, &reg_memory, &reg_flag, &bus_CUflag, &reg_instr, &bus_instr, &reg_instrAddr, &bus_ALUCode, 
	   &reg_ALURes, &reg_ALUTmp, &reg_a, &reg_b, &reg_c, &reg_seg_instr, &reg_seg_stack, &reg_stack_ptr)
{

}

void CPU::define_program(ubyte entry_point, ubyte segment_stack) {
	bus_data->bind(entry_point);
	reg_seg_instr.read32();
	reg_instrAddr.read32();
	bus_data->bind(segment_stack);
	reg_seg_stack.read32();
}

void CPU::start_program() {
	while(bus_data->extract() != bytev(InstrSet::Halt))
		cu.process();
}

END_NS