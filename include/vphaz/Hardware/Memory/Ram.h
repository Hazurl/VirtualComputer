#pragma once


#include <vphaz/config.h>
#include <vphaz/Hardware/Bus.h>
#include <vphaz/Hardware/CPU/CPU.h>
#include <vphaz/Bin.h>
#include <vphaz/Exception/WrongUtilisationRam.h>
#include <vphaz/Hardware/Memory/MemoryControlFlow.h>

BEGIN_NS

template<uxdword size = udword_max>
class Ram : public MemoryControlFlow32 {
public:
	Ram(Bus32* bus_data, Bus32* bus_addr)
		: bus_data(bus_data), bus_addr(bus_addr) {

	}
	
	void read8() override {
		udword addr = us_cast(bus_addr->extract_32());
		byte data = bus_data->extract_8();

//		std::cout << "RAM[" << addr << "]_8 << " << (dword)data << std::endl;
		memory[addr] = data;
	}

	void write8() override {
		udword addr = static_cast<udword>(bus_addr->extract_32());
		byte data = memory[addr];
//		std::cout << "RAM[" << addr << "]_8 >> " << (dword)data << std::endl;
		bus_data->bind_8(data);
	}

	void read16() override {
		udword addr = static_cast<udword>(bus_addr->extract_32());
		word data = bus_data->extract_16();

//		std::cout << "RAM[" << addr << "]_16 << " << (dword)data << std::endl;
		reinterpret_cast<word&>(memory[addr]) = data;
	}

	void write16() override {
		udword addr = static_cast<udword>(bus_addr->extract_32());
		word data = reinterpret_cast<word&>(memory[addr]);
//		std::cout << "RAM[" << addr << "]_16 >> " << (dword)data << std::endl;
		bus_data->bind_16(data);
	}

	void read32() override {
		udword addr = static_cast<udword>(bus_addr->extract_32());
		dword data = bus_data->extract_32();

//		std::cout << "RAM[" << addr << "]_32 << " << (dword)data << std::endl;
		reinterpret_cast<dword&>(memory[addr]) = data;
	}

	void write32() override {
		udword addr = static_cast<udword>(bus_addr->extract_32());
		dword data = reinterpret_cast<dword&>(memory[addr]);
//		std::cout << "RAM[" << addr << "]_32 >> " << (dword)data << std::endl;
		bus_data->bind_32(data);
	}

	void read8h() override {
		throw WrongRamUtilisation();
	}

	void write8h() override {
		throw WrongRamUtilisation();
	}

	void read8hl() override {
		throw WrongRamUtilisation();
	}

	void read8hh() override {
		throw WrongRamUtilisation();
	}

	void write8hl() override {
		throw WrongRamUtilisation();
	}

	void write8hh() override {
		throw WrongRamUtilisation();
	}

	void read16h() override {
		throw WrongRamUtilisation();
	}
	
	void write16h() override {
		throw WrongRamUtilisation();
	}


	udword reserve(udword _size) {
		if(ptr_avalaible + _size > size) {
			return udword_max;
		} else {
			udword ptr = ptr_avalaible;
			ptr_avalaible += _size;
			return ptr;
		}
	}

	void load_at(udword addr, byte value) {
//		std::cout << "RAM[" << addr << "]_8 << " << (dword)value << std::endl;
		memory[addr] = value;
	}

	void load_at(udword addr, word value) {
//		std::cout << "RAM[" << addr << "]_16 << " << (dword)value << std::endl;
		reinterpret_cast<word&>(memory[addr]) = value;
	}

	void load_at(udword addr, dword value) {
//		std::cout << "RAM[" << addr << "]_32 << " << (dword)value<< std::endl;
		reinterpret_cast<dword&>(memory[addr]) = value;
	}

	void clean() {
		ptr_avalaible = 0;
	}

	byte lookup8(udword addr) {
		return memory[addr];
	}

	word lookup16(udword addr) {
		return reinterpret_cast<word&>(memory[addr]);
	}

	dword lookup32(udword addr) {
		return reinterpret_cast<dword&>(memory[addr]);
	}

private:

	Bus32* bus_data;
	Bus32* bus_addr;

	byte memory[size];

	udword ptr_avalaible = 0;
};

END_NS