#include "stdafx.h"
#include "CPU.h"
#include <iostream>
BEGIN_NS

CPU::CPU(Bus* bus_data, Bus* bus_addr, MemoryControlFlow* ram, Processable* out) :
	bus_data(bus_data),
	reg_memory(bus_data, bus_addr),
	reg_a(bus_data, bus_data),
	reg_b(bus_data, bus_data),
	reg_c(bus_data, bus_data),
	reg_instr(bus_data, &bus_instr),
	reg_instrAddr(bus_data, bus_data),
	reg_ALUTmp(bus_data, &bus_ALUTmp),
	reg_ALURes(&bus_ALURes, bus_data),
	reg_flag(&bus_ALUFlag, &bus_CUFlag),
	reg_stack_ptr(bus_data, bus_data),
	reg_seg_instr(bus_data, bus_data),
	reg_seg_stack(bus_data, bus_data),
	alu(&bus_ALUCode, &bus_ALUFlag, &bus_ALURes, bus_data, &bus_ALUTmp),
	cu(ram, &alu, out, &reg_memory, &reg_flag, &bus_CUFlag, &reg_instr, &bus_instr, 
	   &reg_instrAddr, &bus_ALUCode, &reg_ALURes, &reg_ALUTmp, &reg_a, &reg_b, &reg_c, &reg_seg_instr, &reg_seg_stack, &reg_stack_ptr)
{
	// set entry point
	bus_data->bind(0);
	reg_seg_instr.read();
	reg_instrAddr.read();
	bus_data->bind(64);
	reg_seg_stack.read();
	
	std::cout << "Start >> Fibonacci" << std::endl;
	while(bus_data->extract() != static_cast<byte>(CU::OP::END))
		cu.process();

	std::cout << "End" << std::endl;
}

END_NS