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

#include <iostream>

int main() {
	using namespace NAMESPACE;

	Bus bus_data;
	Bus bus_addr;
	Ram<> ram(&bus_data, &bus_addr);
	OutConsole out(&bus_data);

	CPU cpu(&bus_data, &bus_addr, &ram, &out);
	std::cin.get();
	return 0;
}

