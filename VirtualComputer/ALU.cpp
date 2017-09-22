#include "stdafx.h"
#include "ALU.h"

#include <iostream>

BEGIN_NS

ALU::ALU(const Bus* CU, Bus* flag, Bus* output, const Bus* inputA, const Bus* inputB) 
	: CU(CU), flag(flag), output(output), inputA(inputA), inputB(inputB) 
{

}

void ALU::process() {
	switch(static_cast<ALUInstrSet>(CU->extract())) {
	case ALUInstrSet::Add:
	{
		short a = static_cast<short>(inputA->extract());
		short b = static_cast<short>(inputB->extract());
		short res = a + b;
		setflags(!res, res > 0, res > byte_max, res > byte_max);
		output->bind(static_cast<byte>(res));
		break;
	}
	case ALUInstrSet::Sub:
	{
		short a = static_cast<short>(inputA->extract());
		short b = static_cast<short>(inputB->extract());
		short res = a - b;
		setflags(!res, res > 0, res < byte_min, res >= byte_min);
		output->bind(static_cast<byte>(res));
		break;
	}
	case ALUInstrSet::Neg:
	{
		byte a = inputA->extract();
		byte res = -a;
		setflags(!res, res > 0, a == byte_min, a == byte_min);
		output->bind(res);
		break;
	}
	case ALUInstrSet::Mul:
	{
		int a = static_cast<int>(inputA->extract());
		int b = static_cast<int>(inputB->extract());
		int res = a * b;

		setflags(!res, res > 0, res > byte_max || res < byte_min, res > byte_max);

		output->bind(static_cast<byte>(res));
		break;
	}
	case ALUInstrSet::Div:
	{
		byte a = inputA->extract();
		byte b = inputB->extract();
		if(b == 0) {
			setflags(0, (a > 0) ^ (b > 0), 1, 1);
			break;
		}
		byte res = a / b;
		setflags(!res, res > 0, 0, 0);
		output->bind(res);
		break;
	}
	case ALUInstrSet::Mod:
	{
		byte a = inputA->extract();
		byte b = inputB->extract();
		if(b == 0) {
			setflags(0, (a > 0) ^ (b > 0), 1, 1);
			break;
		}
		byte res = a % b;
		setflags(!res, res > 0, 0, 0);
		output->bind(res);
		break;
	}
	case ALUInstrSet::Inc:
	{
		byte a = inputA->extract();
		byte res = a + 1;
		setflags(!res, res > 0, a == byte_max, a == byte_max);
		output->bind(res);
		break;
	}
	case ALUInstrSet::Dec:
	{
		byte a = inputA->extract();
		byte res = a - 1;
		setflags(!res, res > 0, a == byte_min, a != byte_min);
		output->bind(res);
		break;
	}
	case ALUInstrSet::LShift:
	{
		byte a = inputA->extract();
		bool ovf = a & (1 << 8);
		byte res = a << 1;
		setflags(!res, res > 0, ovf, ovf);
		output->bind(res);
		break;
	}
	case ALUInstrSet::RShift:
	{
		byte a = inputA->extract();
		byte res = a >> 1;
		bool ovf = a & 1;
		setflags(!res, res > 0, ovf, !ovf);
		output->bind(res);
		break;
	}
	case ALUInstrSet::VLShift:
	{
		byte a = inputA->extract();
		byte b = inputB->extract();
		byte res = a << b;
		bool ovf = b > 8 || (a & ((~0) << b));
		setflags(!res, res > 0, ovf, ovf);
		output->bind(res);
		break;
	}
	case ALUInstrSet::VRShift:
	{
		byte a = inputA->extract();
		byte b = inputB->extract();
		byte res = a >> b;
		bool ovf = b > 8 || (a & ~((~0) << b));
		setflags(!res, res > 0, ovf, !ovf);
		output->bind(res);
		break;
	}
	case ALUInstrSet::Comp:
	{
		byte res = inputA->extract() - inputB->extract();
		setflags(!res, res > 0, 0, 0);
		break;
	}
	case ALUInstrSet::Comp0:
	{
		byte res = inputA->extract();
		setflags(!res, res > 0, 0, 0);
		break;
	}
	default:
		exit(1);
	};
}

void ALU::setflags(bool zero, bool comp, bool overflow, bool carry) {
	flag->bind(
		(zero		? bytev(ALUFlag::Zero)		: 0) |
		(comp		? bytev(ALUFlag::Comp)		: 0) |
		(overflow	? bytev(ALUFlag::Overflow)	: 0) |
		(carry		? bytev(ALUFlag::Carry)		: 0)
	);
}

END_NS