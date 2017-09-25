#pragma once

#include "config.h"
#include <stdexcept>
#include <string>

BEGIN_NS

class ALUInstrSetUnknown : std::exception {
public:

	byte is;

	ALUInstrSetUnknown(byte is) : is(is) {}

	virtual const char* what() const throw() {
		return (std::string("Instruction unknown: ") + std::to_string((int)is)).c_str();
	}

};

END_NS