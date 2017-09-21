#pragma once

#include "config.h"
#include "MemoryControlFlow.h"
#include "Bus.h"
#include "CU.h"

BEGIN_NS

template<unsigned int size = ubyte_max + 1>
class Ram : public MemoryControlFlow {
public:
	Ram(Bus* bus_data, Bus* bus_addr)
		: bus_data(bus_data), bus_addr(bus_addr) 
	{
		memory[0] = static_cast<byte>(CU::OP::LOADA);
		memory[1] = 6;
		memory[2] = static_cast<byte>(CU::OP::LOADB);
		memory[3] = 7;
		memory[4] = static_cast<byte>(CU::OP::MUL);
		memory[5] = static_cast<byte>(CU::OP::LOADB);
		memory[6] = 42;
		memory[7] = static_cast<byte>(CU::OP::ADD);
		memory[8] = static_cast<byte>(CU::OP::OUTA);
		memory[9] = static_cast<byte>(CU::OP::END);
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

private:

	Bus* bus_data;
	Bus* bus_addr;

	byte memory[size];
};

END_NS