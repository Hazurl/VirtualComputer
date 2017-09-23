#include "stdafx.h"
#include "ALU.h"

#include <iostream>

BEGIN_NS

ALU::ALU(const Bus8* CU, Bus8* flag, Bus32* output, const Bus32* inputA, const Bus32* inputB) 
	: CU(CU), flag(flag), output(output), inputA(inputA), inputB(inputB) 
{

}

void ALU::process() {
	switch(static_cast<ALUInstrSet>(CU->extract())) {
	case ALUInstrSet::Add32:
	{
		xdword a = static_cast<xdword>(inputA->extract());
		xdword b = static_cast<xdword>(inputB->extract());
		xdword res = a + b;
		setflags(!res, res > 0, res > dword_max, res > dword_max);
		output->bind(static_cast<dword>(res));
		break;
	}
	case ALUInstrSet::Sub32:
	{
		xdword a = static_cast<xdword>(inputA->extract());
		xdword b = static_cast<xdword>(inputB->extract());
		xdword res = a - b;
		setflags(!res, res > 0, res < dword_min, res >= dword_min);
		output->bind(static_cast<dword>(res));
		break;
	}
	case ALUInstrSet::Neg32:
	{
		dword a = inputA->extract();
		dword res = -a;
		setflags(!res, res > 0, a == dword_min, a == dword_min);
		output->bind(res);
		break;
	}
	case ALUInstrSet::Mul32:
	{
		xdword a = static_cast<int>(inputA->extract());
		xdword b = static_cast<int>(inputB->extract());
		xdword res = a * b;

		setflags(!res, res > 0, res > dword_max || res < dword_min, res > dword_max);

		output->bind(static_cast<dword>(res));
		break;
	}
	case ALUInstrSet::Div32:
	{
		dword a = inputA->extract();
		dword b = inputB->extract();
		if(b == 0) {
			setflags(0, (a > 0) ^ (b > 0), 1, 1);
			break;
		}
		dword res = a / b;
		setflags(!res, res > 0, 0, 0);
		output->bind(res);
		break;
	}
	case ALUInstrSet::Mod32:
	{
		dword a = inputA->extract();
		dword b = inputB->extract();
		if(b == 0) {
			setflags(0, (a > 0) ^ (b > 0), 1, 1);
			break;
		}
		dword res = a % b;
		setflags(!res, res > 0, 0, 0);
		output->bind(res);
		break;
	}
	case ALUInstrSet::Inc32:
	{
		dword a = inputA->extract();
		dword res = a + 1;
		setflags(!res, res > 0, a == dword_max, a == dword_max);
		output->bind(res);
		break;
	}
	case ALUInstrSet::Dec32:
	{
		dword a = inputA->extract();
		dword res = a - 1;
		setflags(!res, res > 0, a == dword_min, a != dword_min);
		output->bind(res);
		break;
	}
	case ALUInstrSet::LShift32:
	{
		dword a = inputA->extract();
		bool ovf = a & (1 << 1);
		dword res = a << 1;
		setflags(!res, res > 0, ovf, ovf);
		output->bind(res);
		break;
	}
	case ALUInstrSet::RShift32:
	{
		dword a = inputA->extract();
		dword res = a >> 1;
		bool ovf = a & 1;
		setflags(!res, res > 0, ovf, !ovf);
		output->bind(res);
		break;
	}
	case ALUInstrSet::VLShift32:
	{
		dword a = inputA->extract();
		dword b = inputB->extract();
		dword res = a << b;
		bool ovf = b > 31 || (a & ((~0) << b));
		setflags(!res, res > 0, ovf, ovf);
		output->bind(res);
		break;
	}
	case ALUInstrSet::VRShift32:
	{
		dword a = inputA->extract();
		dword b = inputB->extract();
		dword res = a >> b;
		bool ovf = b > 31 || (a & ~((~0) << b));
		setflags(!res, res > 0, ovf, !ovf);
		output->bind(res);
		break;
	}
	case ALUInstrSet::Comp32:
	{
		dword a = inputA->extract();
		dword b = inputB->extract();
		setflags(a == b, a > b, 0, 0);
		break;
	}
	case ALUInstrSet::Comp032:
	{
		dword a = inputA->extract();
		setflags(a == 0, a > 0, 0, 0);
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