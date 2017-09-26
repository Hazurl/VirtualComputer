#pragma once

#include "config.h"

BEGIN_NS
namespace bin {

struct bit_t {
private:
	byte& b;
	ubyte p;
public:

	bit_t(byte& b, ubyte p) : b(b), p(p) {}

	bit_t& operator = (ubyte v) {
		if(v)
			b |= 1 << p;
		else
			b &= ~(1 << p);
		return *this;
	}

	operator bool() {
		return b & (1 << p);
	}
};


static ubyte lsb(uword w) {
	return w & 0xFF;
}

static uword lsb(udword w) {
	return w & 0xFFFF;
}

static ubyte msb(uword w) {
	return (w >> 8) & 0xFF;
}

static uword msb(udword w) {
	return (w >> 16) & 0xFFFF;
}

static ubyte lsb(word w) {
	return lsb(us_cast(w));
}

static uword lsb(dword w) {
	return lsb(us_cast(w));
}

static ubyte msb(word w) {
	return msb(us_cast(w));
}

static uword msb(dword w) {
	return msb(us_cast(w));
}

static uword join(ubyte h, ubyte l) {
	return (static_cast<uword>(h) << 8 | l);
}

static udword join(uword h, uword l) {
	return (static_cast<udword>(h) << 16 | l);
}

static uword join(byte h, byte l) {
	return join(us_cast(h), us_cast(l));
}

static udword join(word h, word l) {
	return join(us_cast(h), us_cast(l));
}

static uword join(ubyte h, byte l) {
	return join(h, us_cast(l));
}

static udword join(uword h, word l) {
	return join(h, us_cast(l));
}

static uword join(byte h, ubyte l) {
	return join(us_cast(h), l);
}

static udword join(word h, uword l) {
	return join(us_cast(h), l);
}

}
END_NS