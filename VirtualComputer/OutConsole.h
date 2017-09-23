#pragma once

#include "config.h"
#include "Processable.h"
#include "Bus.h"

BEGIN_NS

enum class OutConsoleMode {
	Binary,
	Int, 
	Char
};

enum class OutConsoleBetween {
	Space,
	Endl,
	Nothing
};

template<OutConsoleMode mode, OutConsoleBetween between>
class OutConsole : public Processable {
public:

	OutConsole(Bus32* bus_data) : bus_data(bus_data) {}

	void OutConsole::process() {
		dword data = bus_data->extract();
		switch(mode) {
		case vphaz::OutConsoleMode::Binary:
			for(byte i = 31; i >= 0; --i)
				std::cout << ((data & (1 << i)) != 0);
			break;
		case vphaz::OutConsoleMode::Int:
			std::cout << (int)data;
			break;
		case vphaz::OutConsoleMode::Char:
			std::cout << (char)data;
			break;
		}
		
		switch(between) {
		case vphaz::OutConsoleBetween::Space:
			std::cout << " ";
			break;
		case vphaz::OutConsoleBetween::Endl:
			std::cout << std::endl;
			break;
		case vphaz::OutConsoleBetween::Nothing:
			break;
		}
	}

private:

	Bus32* bus_data;
};

using OutConsoleChar = OutConsole<OutConsoleMode::Char, OutConsoleBetween::Nothing>;
using OutConsoleBinary= OutConsole<OutConsoleMode::Binary, OutConsoleBetween::Endl>;

END_NS