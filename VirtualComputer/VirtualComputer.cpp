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

	Bus32 bus_data;
	Bus32 bus_addr;
	Ram<1 << 16> ram(&bus_data, &bus_addr);
	OutConsole<OutConsoleMode::Int, OutConsoleBetween::Space> out(&bus_data);
	//OutConsole<OutConsoleMode::Binary, OutConsoleBetween::Space> out(&bus_data);
	CPU cpu(&bus_data, &bus_addr, &ram, &out);

	if (true) {
		MachineCode code = MachineCodeHelper::generate_counter();
		udword seg_instr, seg_stack;
		dword linker[] = { 10 };

		ram.clean();
		MachineCodeHelper::load_to_ram(&ram, &code, linker, seg_instr, seg_stack);

		cpu.define_program(seg_instr, seg_stack);
		cpu.start_program();
	}
	std::cout << std::endl << std::endl;
	if(true){
		MachineCode code = MachineCodeHelper::generate_for_in_range();
		udword seg_instr, seg_stack;
		dword linker[] = { 120, 130 };

		ram.clean();
		MachineCodeHelper::load_to_ram(&ram, &code, linker, seg_instr, seg_stack);

		cpu.define_program(seg_instr, seg_stack);
		cpu.start_program();
	}
	std::cout << std::endl << std::endl;
	if(true){
		MachineCode code = MachineCodeHelper::generate_fibonacci();
		udword seg_instr, seg_stack;
		dword linker[] = { 44 };

		ram.clean();
		MachineCodeHelper::load_to_ram(&ram, &code, linker, seg_instr, seg_stack);

		cpu.define_program(seg_instr, seg_stack);
		cpu.start_program();
	}

	std::cout << std::endl;
	std::cin.get();
	return 0;
}

