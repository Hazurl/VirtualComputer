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

using namespace NAMESPACE;

template<unsigned int RS>
void startCode(MachineCode code, Ram<RS>* ram, CPU* cpu) {
	udword seg_instr, seg_data, seg_bss, seg_stack, entry_point;

	ram->clean();
	MachineCodeHelper::load_to_ram(ram, &code, seg_instr, seg_data, seg_bss, seg_stack, entry_point);
	/*
	std::cout << "Segments: " << std::endl;
	std::cout << "\tInstructions " << seg_instr << std::endl;
	std::cout << "\tData " << seg_data << std::endl;
	std::cout << "\tBss " << seg_bss << std::endl;
	std::cout << "\tStack " << seg_stack << std::endl;
	*/
	cpu->define_program(seg_instr, seg_data, seg_bss, seg_stack, entry_point);
	cpu->start_program();
}

int main() {
	Bus32 bus_data;
	Bus32 bus_addr;
	Ram<1 << 16> ram(&bus_data, &bus_addr);
	OutConsole out(OutConsoleMode::Int, OutConsoleBetween::Space, &bus_data);
	//OutConsole<OutConsoleMode::Binary, OutConsoleBetween::Space> out(&bus_data);
	CPU cpu(&bus_data, &bus_addr, &ram, &out);

	if (true)
		startCode(MachineCodeHelper::generate_counter(), &ram, &cpu);

	std::cout << std::endl << std::endl;

	if(true)
		startCode(MachineCodeHelper::generate_for_in_range(), &ram, &cpu);

	std::cout << std::endl << std::endl;

	if(true)
		startCode(MachineCodeHelper::generate_fibonacci(), &ram, &cpu);

	std::cout << std::endl << std::endl;

	out.change_mode(OutConsoleMode::Char, OutConsoleBetween::Nothing);
	if(true)
		startCode(MachineCodeHelper::generate_hello_world(), &ram, &cpu);

	std::cout << std::endl;
	std::cin.get();
	return 0;
}

