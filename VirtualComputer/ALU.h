#pragma once

#include "config.h"
#include "Bus.h"
#include "Processable.h"
#include "InstrSet.h"

BEGIN_NS

class ALU : public Processable {
public:
	ALU(const Bus* CU, Bus* flag, Bus* output, const Bus* inputA, const Bus* inputB);

	void process() override;

private:

	void setflags(bool zero, bool comp, bool overflow, bool carry);

	const Bus* CU;
	Bus* flag;
	Bus* output;
	const Bus* inputA;
	const Bus* inputB;

};

END_NS