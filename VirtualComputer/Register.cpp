#include "stdafx.h"
#include "Register.h"

BEGIN_NS

Register::Register(const Bus* input, Bus* output) : input(input), output(output) {

}

void Register::enable() {
	if(output)
		output->bind(storage);
}

void Register::set() {
	if(input)
		storage = input->extract();
}

END_NS