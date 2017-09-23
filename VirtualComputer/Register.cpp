#include "stdafx.h"
#include "Register.h"

BEGIN_NS

// Register 8

Register8::Register8(const Bus8 * input, Bus8 * output) : input(input), output(output) {

}

void Register8::write8() {
	output->bind(storage);
}

void Register8::read8() {
	storage = input->extract();
}

// Register 16

Register16::Register16(const Bus16* input, Bus16* output) : input(input), output(output) {

}

void Register16::write16() {
	output->bind(storage);
}

void Register16::read16() {
	storage = input->extract();
}

void Register16::read8() {
	storage = (storage & 0xFF00) | input->extract_l();
}

void Register16::read8h() {
	storage = (storage & 0x00FF) | input->extract_h();
}

void Register16::write8() {
	output->bind_l(storage & 0xFF00);
}

void Register16::write8h() {
	output->bind_h(storage & 0x00FF);
}

// Register 32

Register32::Register32(const Bus32* input, Bus32* output) : input(input), output(output) {

}

void Register32::write32() {
	output->bind(storage);
}

void Register32::read32() {
	storage = input->extract();
}

void Register32::read8() {
	storage = (storage & 0xFFFFFF00) | input->extract_ll();
}

void Register32::read8h() {
	storage = (storage & 0xFFFF00FF) | input->extract_lh();
}

void Register32::write8() {
	output->bind_ll(storage & 0x000000FF);
}

void Register32::write8h() {
	output->bind_ll(storage & 0x0000FF00);
}

void Register32::read16() {
	storage = (storage & 0xFFFF0000) | input->extract_l();
}

void Register32::read16h() {
	storage = (storage & 0x0000FFFF) | input->extract_h();
}

void Register32::write16() {
	output->bind_ll(storage & 0x0000FFFF);
}

void Register32::write16h() {
	output->bind_ll(storage & 0xFFFF0000);
}

END_NS