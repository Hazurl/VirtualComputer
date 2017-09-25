#pragma once

#include "config.h"
#include "Bin.h"

BEGIN_NS

class Bus8 {
public:

	void bind_8(byte b);
	byte extract_8() const;
	
	bin::bit_t bit(ubyte b);

private:
	byte storage;
};

class Bus16 : public Bus8 {
public:
	void bind_8(byte b);
	void bind_16(word b);

	byte extract_8() const;
	word extract_16() const;

	bin::bit_t bit(ubyte p);

private:

	byte h;
};

class Bus32 : public Bus16 {
public:
	void bind_8(byte ll);
	void bind_16(word h);
	void bind_32(dword b);

	byte extract_8() const;
	word extract_16() const;
	dword extract_32() const;

	bin::bit_t bit(ubyte p);

private:

	word h;
};

END_NS