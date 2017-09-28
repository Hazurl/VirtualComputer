#pragma once

#include <vphaz/config.h>
#include <stdexcept>
#include <string>

BEGIN_NS

class WrongRamUtilisation : std::exception {
public:
	virtual const char* what() const throw() {
		return "Wrong utilisation of the ram";
	}

};

END_NS