#include "stdafx.h"
#include "Register.h"

BEGIN_NS

Register8::Register8(const Bus8* input, Bus8* output) : input(input), output(output) {

}

void Register8::write() {
	output->bind(storage);
}

void Register8::read() {
	storage = input->extract();
}

Register16::Register16(const Bus16* input, Bus16* output) : input(input), output(output) {

}

void Register16::write() {
	output->bind(storage);
}

void Register16::read() {
	storage = input->extract();
}

Register32::Register32(const Bus32* input, Bus32* output) : input(input), output(output) {

}

void Register32::write() {
	output->bind(storage);
}

void Register32::read() {
	storage = input->extract();
}

END_NS