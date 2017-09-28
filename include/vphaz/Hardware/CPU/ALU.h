#pragma once

#include <vphaz/config.h>
#include <vphaz/Hardware/Bus.h>
#include <vphaz/Hardware/Processable.h>
#include <vphaz/Hardware/CPU/InstrSet.h>
#include <vphaz/Exception/ALUInstrSetUnknown.h>

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