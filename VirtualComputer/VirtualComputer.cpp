// VirtualComputer.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "ALU.h"
#include "Bus.h"
#include "Bus.h"
#include "Register.h"
#include "CPU.h"
#include "Ram.h"
#include "OutConsole.h"
#include "MachineCode.h"
#include "MachineCodeHelper.h"

#include <iostream>
#include <vector>

int main() {
	using namespace NAMESPACE;

	Bus bus_data;
	Bus bus_addr;
	Ram<> ram(&bus_data, &bus_addr);
	OutConsole out(&bus_data);
	CPU cpu(&bus_data, &bus_addr, &ram, &out);

	MachineCode code = MachineCodeHelper::generate_fibonacci();
	ubyte seg_instr, seg_stack;
	byte linker[1] = { 9 };

	ram.clean();
	MachineCodeHelper::load_to_ram(&ram, &code, linker, seg_instr, seg_stack);

	cpu.define_program(seg_instr, seg_stack);
	cpu.start_program();

	std::cin.get();
	return 0;
}

