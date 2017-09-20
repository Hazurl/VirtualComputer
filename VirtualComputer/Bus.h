#pragma once

#include "config.h"

BEGIN_NS

class Bus {
public:

	void bind(byte b);
	byte extract() const;

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
	byte storage;
};

END_NS