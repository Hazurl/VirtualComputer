#include "stdafx.h"
#include "Register.h"

BEGIN_NS

Register::Register(const Bus* input, Bus* output) : input(input), output(output) {

}

void Register::enable() {
	output->bind(storage);
}

void Register::set() {
	storage = input->extract();
}

END_NS