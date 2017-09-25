#include "stdafx.h"
#include "Register.h"

BEGIN_NS

// Register 8

Register8::Register8(const Bus8 * input, Bus8 * output) : input(input), output(output) {

}

void Register8::write8() {
	output->bind_8(storage);
}

void Register8::read8() {
	storage = input->extract_8();
}

// Register 16

Register16::Register16(const Bus16* input, Bus16* output) : input(input), output(output) {

}

void Register16::write16() {
	output->bind_16(storage);
}

void Register16::read16() {
	storage = input->extract_16();
}

void Register16::read8h() {
	storage = bin::join(input->extract_8(), bin::lsb(storage));
}

void Register16::write8h() {
	output->bind_8(bin::msb(storage));
}

void Register16::read8() {
	storage = bin::join(bin::msb(storage), input->extract_8());
}

void Register16::write8() {
	output->bind_8(bin::lsb(storage));
}

// Register 32

Register32::Register32(const Bus32* input, Bus32* output) : input(input), output(output) {

}

void Register32::write32() {
	output->bind_32(storage);
}

void Register32::read32() {
	storage = input->extract_32();
}

void Register32::read8() {
	storage = bin::join(
		bin::msb(storage), bin::join(
		bin::msb(bin::lsb(storage)), 
		input->extract_8()));
}

void Register32::read8h() {
	storage = bin::join(
		bin::msb(storage), bin::join(
		input->extract_8(), 
		bin::lsb(bin::lsb(storage))));
}

void Register32::write8() {
	output->bind_8(bin::lsb(bin::lsb(storage)));
}

void Register32::write8h() {
	output->bind_8(bin::msb(bin::lsb(storage)));
}

void Register32::read8hl() {
	storage = bin::join(bin::join(
		bin::msb(bin::msb(storage)), 
		input->extract_8()), 
		bin::lsb(storage));
}

void Register32::read8hh() {
	storage = bin::join(bin::join(
		input->extract_8(),
		bin::lsb(bin::msb(storage))), 
		bin::lsb(storage));
}

void Register32::write8hl() {
	output->bind_8(bin::lsb(bin::msb(storage)));
}

void Register32::write8hh() {
	output->bind_8(bin::msb(bin::msb(storage)));
}

void Register32::read16() {
	storage = bin::join(bin::msb(storage), input->extract_16());
}

void Register32::read16h() {
	storage = bin::join(input->extract_16(), bin::lsb(storage));
}

void Register32::write16() {
	output->bind_16(bin::lsb(storage));
}

void Register32::write16h() {
	output->bind_16(bin::msb(storage));
}

END_NS