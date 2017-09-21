#include "stdafx.h"
#include "OutConsole.h"
#include <iostream>

BEGIN_NS

OutConsole::OutConsole(Bus* bus_data) : bus_data(bus_data) {}


void OutConsole::process() {
	byte data = bus_data->extract();

	std::cout << "OUT: " << (int)data << " (0b";
	for(byte i = 7; i >= 0; --i)
		std::cout << ((data & (1 << i)) != 0);
	std::cout << ")" << std::endl;
}

END_NS