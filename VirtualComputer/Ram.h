#pragma once

#include "config.h"
#include "MemoryControlFlow.h"
#include "Bus.h"
#include "CU.h"

BEGIN_NS

template<unsigned int size = ubyte_max + 1>
class Ram : public MemoryControlFlow {
public:
	Ram(Bus16* bus_data, Bus16* bus_addr)
		: bus_data(bus_data), bus_addr(bus_addr) {

	}
	
	void read() override {
		ubyte addr = static_cast<ubyte>(bus_addr->extract());
		byte data = bus_data->extract();
		memory[addr] = data;
	}

	void write() override {
		ubyte addr = static_cast<ubyte>(bus_addr->extract());
		byte data = memory[addr];
		bus_data->bind(data);
	}

	ubyte reserve(ubyte _size) {
		if(ptr_avalaible + _size > size) {
			return ubyte_max;
		} else {
			ubyte ptr = ptr_avalaible;
			ptr_avalaible += _size;
			return ptr;
		}
	}

	void load_at(ubyte addr, byte value) {
		memory[addr] = value;
	}

	void clean() {
		ptr_avalaible = 0;
	}

private:

	Bus16* bus_data;
	Bus16* bus_addr;

	byte memory[size];

	ubyte ptr_avalaible = 0;
};

END_NS