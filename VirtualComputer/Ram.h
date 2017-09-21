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
		: bus_data(bus_data), bus_addr(bus_addr) {

		memory[0] = static_cast<byte>(CU::OP::LOADA);
		memory[1] = 1;
		memory[2] = static_cast<byte>(CU::OP::LOADB);
		memory[3] = 1;
		memory[4] = static_cast<byte>(CU::OP::LOADC);
		memory[5] = 9;
		memory[6] = static_cast<byte>(CU::OP::COMPC0);
		memory[7] = static_cast<byte>(CU::OP::JMPLE);
		memory[8] = 16;
		memory[9] = static_cast<byte>(CU::OP::OUTA);
		memory[10] = static_cast<byte>(CU::OP::PUSHA);
		memory[11] = static_cast<byte>(CU::OP::ADD);
		memory[12] = static_cast<byte>(CU::OP::POPB);
		memory[13] = static_cast<byte>(CU::OP::DECCOUNTER);
		memory[14] = static_cast<byte>(CU::OP::JMP);
		memory[15] = 6;
		memory[16] = static_cast<byte>(CU::OP::OUTA);
		memory[17] = static_cast<byte>(CU::OP::END);
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