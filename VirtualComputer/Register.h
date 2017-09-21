#pragma once

#include "config.h"
#include "Bus.h"
#include "MemoryControlFlow.h"

BEGIN_NS

class Register : public MemoryControlFlow {
public:

	Register(const Bus* input, Bus* output);

	void read() override;
	void write() override;

private:

	const Bus* input;
	Bus* output;
	byte storage;
};

END_NS