#pragma once

#include <vphaz/config.h>
#include <vphaz/Hardware/Bus.h>
#include <vphaz/Hardware/Memory/MemoryControlFlow.h>
#include <vphaz/Bin.h>

BEGIN_NS

class Register8 : public MemoryControlFlow8 {
public:

	Register8(const Bus8* input, Bus8* output);

	void read8() override;
	void write8() override;

private:

	const Bus8* input;
	Bus8* output;
	byte storage;
};


class Register16 : public MemoryControlFlow16 {
public:

	Register16(const Bus16* input, Bus16* output);

	void read8() override;
	void read8h() override;
	void write8() override;
	void write8h() override;

	void read16() override;
	void write16() override;

private:

	const Bus16* input;
	Bus16* output;
	word storage;
};

class Register32 : public MemoryControlFlow32 {
public:

	Register32(const Bus32* input, Bus32* output);

	void read8() override;
	void read8h() override;
	void write8() override;
	void write8h() override;

	void read8hl() override;
	void read8hh() override;
	void write8hl() override;
	void write8hh() override;

	void read16() override;
	void read16h() override;
	void write16() override;
	void write16h() override;

	void read32() override;
	void write32() override;

private:

	const Bus32* input;
	Bus32* output;
	dword storage;
};

END_NS