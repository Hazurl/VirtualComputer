#pragma once

#include "config.h"
#include <stdexcept>
#include <string>

BEGIN_NS

class ALUInstrSetUnknown : std::exception {
public:

	std::string str;

	ALUInstrSetUnknown(byte is) : str(std::string("Instruction unknown: ") + std::to_string((int)is)) {}

	virtual const char* what() const throw() {
		return str.c_str();
	}

};

END_NS