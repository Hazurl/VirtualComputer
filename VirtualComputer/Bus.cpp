#include "stdafx.h"
#include "Bus.h"

BEGIN_NS

/*
 * Bus 8
 */

void Bus8::bind_8(byte b) {
	storage = b;
}

byte Bus8::extract_8() const {
	return storage;
}


Bus8::bit_t Bus8::bit(ubyte b) {
	return bit_t(storage, b);
}

/*
 * Bus 16
 */

void Bus16::bind_16(word b) {
	l.bind_8(b & 0xFF);
	h.bind_8(b >> 8);
}

void Bus16::bind_8(byte b) {
	l.bind_8(b);
}

byte Bus16::extract_8() const {
	return l.extract_8();
}

word Bus16::extract_16() const {
	return (static_cast<ubyte>(h.extract_8()) << 8) | static_cast<ubyte>(l.extract_8());
}

Bus8::bit_t Bus16::bit(ubyte p) {
	if(p > 7)
		return h.bit(p - 8);
	return l.bit(p);
}

Bus8& Bus16::bus_8() {
	return l;
}

/*
* Bus 32
*/

void Bus32::bind_8(byte ll) {
	l.bind_8(ll);
}

void Bus32::bind_16(word l) {
	this->l.bind_16(l);
}

void Bus32::bind_32(dword b) {
	h.bind_16(b >> 16);
	l.bind_16(b & 0xFFFF);
}

byte Bus32::extract_8() const {
	return l.extract_8();
}

word Bus32::extract_16() const {
	return l.extract_16();
}

dword Bus32::extract_32() const {
	return (static_cast<uword>(h.extract_16()) << 16) | static_cast<uword>(l.extract_16());
}

Bus8::bit_t Bus32::bit(ubyte p) {
	if(p > 15)
		return h.bit(p - 16);
	return l.bit(p);
}

Bus8& Bus32::bus_8() {
	return l.bus_8();
}

Bus16& Bus32::bus_16() {
	return l;
}

END_NS

