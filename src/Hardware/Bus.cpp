#include <vphaz/Hardware/Bus.h>

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


bin::bit_t Bus8::bit(ubyte b) {
	return bin::bit_t(storage, b);
}

/*
 * Bus 16
 */

void Bus16::bind_16(word b) {
	Bus8::bind_8(bin::lsb(us_cast(b)));
	h = bin::msb(us_cast(b));
}

void Bus16::bind_8(byte b) {
	Bus8::bind_8(b);
}

byte Bus16::extract_8() const {
	return Bus8::extract_8();
}

word Bus16::extract_16() const {
	return bin::join(h, Bus8::extract_8());
}

bin::bit_t Bus16::bit(ubyte p) {
	if(p > 7)
		return bin::bit_t(h, p - 8);
	return Bus8::bit(p);
}

/*
* Bus 32
*/

void Bus32::bind_8(byte b) {
	Bus16::bind_8(b);
}

void Bus32::bind_16(word b) {
	Bus16::bind_16(b);
}

void Bus32::bind_32(dword b) {
	h = bin::msb(b);
	Bus16::bind_16(bin::lsb(b));
}

byte Bus32::extract_8() const {
	return Bus16::extract_8();
}

word Bus32::extract_16() const {
	return Bus16::extract_16();
}

dword Bus32::extract_32() const {
	return bin::join(h, Bus16::extract_16());
}

bin::bit_t Bus32::bit(ubyte p) {
	if(p > 15) {
		if(p > 23)
			return bin::bit_t(*reinterpret_cast<byte*>((&h) + sizeof(byte)), p - 16);
		else
			return bin::bit_t(reinterpret_cast<byte&>(h), p - 24);
	}
	return Bus16::bit(p);
}

END_NS

