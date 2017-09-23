#pragma once

#include "config.h"

BEGIN_NS

class Bus8 {
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
	byte storage = 0;
};

class Bus16 {
public:
	void bind(byte h, byte l);
	void bind_h(byte b);
	void bind_l(byte b);
	void bind(word b);

	byte extract_h() const;
	byte extract_l() const;
	word extract() const;

	Bus8::bit_t bit(ubyte p);

	Bus8& bus_h();
	Bus8& bus_l();

private:

	Bus8 h;
	Bus8 l;
};

class Bus32 {
public:
	void bind(byte hh, byte hl, byte lh, byte ll);
	void bind_hh(byte hh);
	void bind_hl(byte hl);
	void bind_lh(byte lh);
	void bind_ll(byte ll);
	void bind(word h, word l);
	void bind_h(word h);
	void bind_l(word l);
	void bind(dword b);

	byte extract_hh() const;
	byte extract_hl() const;
	byte extract_lh() const;
	byte extract_ll() const;
	word extract_h() const;
	word extract_l() const;
	dword extract() const;

	Bus8::bit_t bit(ubyte p);

	Bus8& bus_hh();
	Bus8& bus_hl();
	Bus8& bus_lh();
	Bus8& bus_ll();
	Bus16& bus_h();
	Bus16& bus_l();

private:

	Bus16 h;
	Bus16 l;
};

END_NS