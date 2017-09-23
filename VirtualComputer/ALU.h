#pragma once

#include "config.h"
#include "Bus.h"
#include "Processable.h"
#include "InstrSet.h"

BEGIN_NS

class ALU : public Processable {
public:
	ALU(const Bus16* CU, Bus16* flag, Bus16* output, const Bus16* inputA, const Bus16* inputB);

	void process() override;

private:

	void setflags(bool zero, bool comp, bool overflow, bool carry);

	const Bus16* CU;
	Bus16* flag;
	Bus16* output;
	const Bus16* inputA;
	const Bus16* inputB;

};

END_NS