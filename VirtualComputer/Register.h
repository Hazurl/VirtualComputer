#pragma once

#include "config.h"
#include "Bus.h"

BEGIN_NS

class Register {
public:

	Register(const Bus* input, Bus* output);

	void enable();
	void set();

private:

	const Bus* input;
	Bus* output;
	byte storage;
};

END_NS