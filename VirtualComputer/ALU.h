#pragma once

#include "config.h"
#include "Bus.h"
#include "Processable.h"

BEGIN_NS

class ALU : public Processable {
public:
	ALU(const Bus* CU, Bus* flag, Bus* output, const Bus* inputA, const Bus* inputB);

	enum class CU_CODE : byte {
		ADD = byte_min, SUB, // a+b a-b
		MUL, DIV, // a*b a/b
		NEG, MOD, // -a a%b
		INC, DEC, // ++a --a
		LSHFT, RSHFT, // a<<1 a>>1
		VLSHFT, VRSHFT, // a<<b a>>b
		COMP, // a > b ? a == b ?
	};

	enum class FLAG_BIT : ubyte {
		Zero, // 1 if equals
		Comp, // 1 if A > B
		Overflow, // 1 if overflow/underflow
		Carry, // 1 is overflow, 0 is underflow
	};

	ubyte bit(FLAG_BIT b) const;

	void process() override;

private:

	void setflag(FLAG_BIT f, ubyte v);

	const Bus* CU;
	Bus* flag;
	Bus* output;
	const Bus* inputA;
	const Bus* inputB;

};

END_NS