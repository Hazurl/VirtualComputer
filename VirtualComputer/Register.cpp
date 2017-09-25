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
	storage = (storage & 0x00FF) | input->extract_8();
}

void Register16::write8h() {
	output->bind_8((storage & 0xFF00) >> 8);
}

void Register16::read8() {
	storage = (storage & 0xFF00) | (input->extract_8() << 8);
}

void Register16::write8() {
	output->bind_8(storage & 0x00FF);
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
	storage = (storage & 0xFFFFFF00) | input->extract_8();
}

void Register32::read8h() {
	storage = (storage & 0xFFFF00FF) | (input->extract_8() << 8);
}

void Register32::write8() {
	output->bind_8(storage & 0x000000FF);
}

void Register32::write8h() {
	output->bind_8((storage & 0x0000FF00) >> 8);
}

void Register32::read8hl() {
	storage = (storage & 0xFF00FFFF) | (input->extract_8() << 16);
}

void Register32::read8hh() {
	storage = (storage & 0x00FFFFFF) | (input->extract_8() << 24);
}

void Register32::write8hl() {
	output->bind_8((storage & 0x00FF0000) >> 16);
}

void Register32::write8hh() {
	output->bind_8((storage & 0xFF000000) >> 24);
}

void Register32::read16() {
	storage = (storage & 0xFFFF0000) | input->extract_16();
}

void Register32::read16h() {
	storage = (storage & 0x0000FFFF) | (input->extract_16() << 16);
}

void Register32::write16() {
	output->bind_16(storage & 0x0000FFFF);
}

void Register32::write16h() {
	output->bind_16((storage & 0xFFFF0000) >> 16);
}

END_NS