// VirtualComputer.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "ALU.h"
#include "Bus.h"
#include "Bus.h"
#include "Register.h"
#include "CPU.h"

#include <iostream>

#define dumb_bus \
	std::cout << "\tCU_bus: " << static_cast<int>(CU_bus.extract()) << std::endl;\
	std::cout << "\tIA_bus: " << static_cast<int>(IA_bus.extract()) << std::endl;\
	std::cout << "\tIB_bus: " << static_cast<int>(IB_bus.extract()) << std::endl;\
	std::cout << "\tO_bus: " << static_cast<int>(O_bus.extract()) << std::endl;\
	std::cout << "\tflag_bus: " << static_cast<int>(flag_bus.extract()) << std::endl;

int main() {
	using namespace NAMESPACE;

	Bus bus_data;
	CPU cpu(&bus_data);
	std::cin.get();
	return 0;
}

