#include "stdafx.h"
#include "CPU.h"
#include <iostream>
BEGIN_NS

CPU::CPU(Bus* bus_data) :
	reg_a(bus_data, bus_data),
	reg_b(bus_data, bus_data),
	reg_c(bus_data, bus_data),
	reg_instr(bus_data, &bus_instr),
	reg_instrAddr(bus_data, bus_data),
	reg_ALUTmp(bus_data, &bus_ALUTmp),
	reg_ALURes(&bus_ALURes, bus_data),
	reg_flag(&bus_ALUFlag, &bus_CUFlag),
	alu(&bus_ALUCode, &bus_ALUFlag, &bus_ALURes, bus_data, &bus_ALUTmp),
	cu(&alu, &reg_flag, &bus_CUFlag, &reg_instr, &bus_instr, &reg_instrAddr, &bus_ALUCode, &reg_ALURes, &reg_ALUTmp, &reg_a, &reg_b, &reg_c)
{
	
	// Put ... in A
	bus_data->bind(static_cast<byte>(CU::OP::LOADA));
	reg_instr.read();
	reg_instr.write();
	bus_data->bind(6);
	cu.process();

	// Put ... in B
	bus_data->bind(static_cast<byte>(CU::OP::LOADB));
	reg_instr.read();
	reg_instr.write();
	bus_data->bind(7);
	cu.process();

	// Out A
	bus_data->bind(static_cast<byte>(CU::OP::OUTA));
	reg_instr.read();
	reg_instr.write();
	cu.process();

	std::cout << "Register A: " << (int)bus_data->extract() << std::endl;

	// Out b
	bus_data->bind(static_cast<byte>(CU::OP::OUTB));
	reg_instr.read();
	reg_instr.write();
	cu.process();
	std::cout << "Register B: " << (int)bus_data->extract() << std::endl;

	// Put A + B in A
	bus_data->bind(static_cast<byte>(CU::OP::MUL));
	reg_instr.read();
	reg_instr.write();
	cu.process();

	// Out A
	bus_data->bind(static_cast<byte>(CU::OP::OUTA));
	reg_instr.read();
	reg_instr.write();
	cu.process();

	std::cout << "Register A: " << (int)bus_data->extract() << std::endl;
}

END_NS