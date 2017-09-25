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


ubyte lsb(uword w) {
	return w & 0x00FF;
}

uword lsb(udword w) {
	return 0x0000FFFF;
}

ubyte msb(uword w) {
	return w & 0xFF00;
}

uword msb(udword w) {
	return 0xFFFF0000;
}

inline ubyte lsb(word w) {
	return lsb(us_cast(w));
}

inline uword lsb(dword w) {
	return lsb(us_cast(w));
}

inline ubyte msb(word w) {
	return msb(us_cast(w));
}

inline uword msb(dword w) {
	return msb(us_cast(w));
}

uword join(ubyte h, ubyte l) {
	return (static_cast<uword>(h) << 8 | l);
}

udword join(uword h, uword l) {
	return (static_cast<udword>(h) << 16 | l);
}

inline uword join(byte h, byte l) {
	return join(us_cast(h), us_cast(l));
}

inline udword join(word h, word l) {
	return join(us_cast(h), us_cast(l));
}

inline uword join(ubyte h, byte l) {
	return join(h, us_cast(l));
}

inline udword join(uword h, word l) {
	return join(h, us_cast(l));
}

inline uword join(byte h, ubyte l) {
	return join(us_cast(h), l);
}

inline udword join(word h, uword l) {
	return join(us_cast(h), l);
}

}
END_NS