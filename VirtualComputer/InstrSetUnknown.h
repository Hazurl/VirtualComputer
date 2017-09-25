#pragma once

#include "config.h"
#include <stdexcept>
#include <string>

BEGIN_NS

class InstrSetUnknown : std::exception {
public:

	dword is;

	InstrSetUnknown(dword is) : is(is) {}

	virtual const char* what() const throw() {
		return (std::string("Instruction unknown: ") + std::to_string(is)).c_str();
	}

};

END_NS