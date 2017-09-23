#pragma once

#include "config.h"
#include "Bus.h"
#include "MemoryControlFlow.h"

BEGIN_NS

class Register8 : public MemoryControlFlow {
public:

	Register8(const Bus8* input, Bus8* output);

	void read() override;
	void write() override;

private:

	const Bus8* input;
	Bus8* output;
	byte storage;
};


class Register16 : public MemoryControlFlow {
public:

	Register16(const Bus16* input, Bus16* output);

	void read() override;
	void write() override;

private:

	const Bus16* input;
	Bus16* output;
	word storage;
};

class Register32 : public MemoryControlFlow {
public:

	Register32(const Bus32* input, Bus32* output);

	void read() override;
	void write() override;

private:

	const Bus32* input;
	Bus32* output;
	dword storage;
};

END_NS