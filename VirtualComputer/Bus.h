#pragma once

#include "config.h"

BEGIN_NS

class Bus8 {
public:

	void bind_8(byte b);
	byte extract_8() const;

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
	
	bit_t bit(ubyte b);

private:
	byte storage = 0;
};

class Bus16 {
public:
	void bind_8(byte b);
	void bind_16(word b);

	byte extract_8() const;
	word extract_16() const;

	Bus8::bit_t bit(ubyte p);

	Bus8& bus_8();

private:

	Bus8 h;
	Bus8 l;
};

class Bus32 {
public:
	void bind_8(byte ll);
	void bind_16(word h);
	void bind_32(dword b);

	byte extract_8() const;
	word extract_16() const;
	dword extract_32() const;

	Bus8::bit_t bit(ubyte p);

	Bus8& bus_8();
	Bus16& bus_16();

private:

	Bus16 h;
	Bus16 l;
};

END_NS