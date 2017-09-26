#pragma once

#include "config.h"
#include <stdexcept>
#include <string>

BEGIN_NS

class InstrSetUnknown : std::exception {
public:

	std::string str;

	InstrSetUnknown(dword is) : str(std::string("Instruction unknown: ") + std::to_string((int)is)) {}

	virtual const char* what() const throw() {
		return str.c_str();
	}

};

END_NS