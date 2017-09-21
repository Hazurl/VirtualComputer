#include "stdafx.h"
#include "Register.h"

BEGIN_NS

Register::Register(const Bus* input, Bus* output) : input(input), output(output) {

}

void Register::write() {
	output->bind(storage);
}

void Register::read() {
	storage = input->extract();
}

END_NS