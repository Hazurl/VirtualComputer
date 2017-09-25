#pragma once

#include "config.h"
#include "MemoryControlFlow.h"
#include "Bus.h"
#include "CU.h"
#include "Bin.h"
#include "WrongUtilisationRam.h"

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

		memory[addr] = data;
	}

	void write8() override {
		udword addr = static_cast<udword>(bus_addr->extract());
		byte data = memory[addr];
		bus_data->bind_ll(data);
	}

	void read16() override {
		udword addr = static_cast<udword>(bus_addr->extract());
		word data = bus_data->extract_l();

		reinterpret_cast<word&>(memory[addr]) = data;
	}

	void write16() override {
		udword addr = static_cast<udword>(bus_addr->extract());
		word data = reinterpret_cast<word&>(memory[addr]);
		bus_data->bind_l(data);
	}

	void read32() override {
		udword addr = static_cast<udword>(bus_addr->extract());
		dword data = bus_data->extract();

		reinterpret_cast<dword&>(memory[addr]) = data;
	}

	void write32() override {
		udword addr = static_cast<udword>(bus_addr->extract());
		dword data = reinterpret_cast<dword&>(memory[addr]);
		bus_data->bind(data);
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
		memory[addr] = value;
	}

	void load_at(udword addr, word value) {
		reinterpret_cast<word&>(memory[addr]) = value;
	}

	void load_at(udword addr, dword value) {
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