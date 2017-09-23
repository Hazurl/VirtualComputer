#pragma once

#include "config.h"
#include "Bus.h"
#include "Processable.h"
#include "InstrSet.h"

BEGIN_NS

class ALU : public Processable {
public:
	ALU(const Bus8* CU, Bus8* flag, Bus32* output, const Bus32* inputA, const Bus32* inputB);

	void process() override;

private:

	void setflags(bool zero, bool comp, bool overflow, bool carry);

	const Bus8* CU;
	Bus8* flag;
	Bus32* output;
	const Bus32* inputA;
	const Bus32* inputB;

};

END_NS