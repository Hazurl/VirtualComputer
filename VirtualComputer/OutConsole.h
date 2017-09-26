#pragma once

#include "config.h"
#include "Processable.h"
#include "Bus.h"

#include <iostream>

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

class OutConsole : public Processable {
public:

	OutConsole(OutConsoleMode mode, OutConsoleBetween bet, Bus32* bus_data) : bus_data(bus_data), mode(mode), bet(bet) {}

	void OutConsole::process() {
		dword data = bus_data->extract_32();
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
		
		switch(bet) {
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

	void change_mode(OutConsoleMode mode, OutConsoleBetween bet) {
		this->mode = mode;
		this->bet = bet;
	}

private:

	Bus32* bus_data;

	OutConsoleMode mode;
	OutConsoleBetween bet;
};

END_NS