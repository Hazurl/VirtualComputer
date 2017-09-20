// VirtualComputer.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "ALU.h"
#include "Bus.h"
#include "Bus.h"
#include "Register.h"
#include <iostream>

#define dumb_bus \
	std::cout << "\tCU_bus: " << static_cast<int>(CU_bus.extract()) << std::endl;\
	std::cout << "\tIA_bus: " << static_cast<int>(IA_bus.extract()) << std::endl;\
	std::cout << "\tIB_bus: " << static_cast<int>(IB_bus.extract()) << std::endl;\
	std::cout << "\tO_bus: " << static_cast<int>(O_bus.extract()) << std::endl;\
	std::cout << "\tflag_bus: " << static_cast<int>(flag_bus.extract()) << std::endl;

int main() {
	using namespace NAMESPACE;

	Bus flag_bus;
	Bus IA_bus;
	Bus IB_bus;
	Bus O_bus;
	Bus CU_bus;

	ALU alu(&CU_bus, &flag_bus, &O_bus, &IA_bus, &IB_bus);
	
	flag_bus.bind(0);
	IA_bus.bind(16);
	IB_bus.bind(5);
	O_bus.bind(0);
	CU_bus.bind(static_cast<byte>(ALU::CU_CODE::VRSHFT));

	std::cout << "Before: " << std::endl;
	dumb_bus
	alu.process();
	std::cout << "After: " << std::endl;
	dumb_bus
	std::cin.get();
    return 0;
}

