#include "stdafx.h"
#include "Bus.h"

BEGIN_NS

void Bus::bind(byte b) {
	storage = b;
}

byte Bus::extract() const {
	return storage;
}


Bus::bit_t Bus::bit(ubyte b) {
	return bit_t(storage, b);
}

END_NS