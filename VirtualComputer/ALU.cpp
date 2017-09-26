#include "stdafx.h"
#include "ALU.h"

#include <iostream>

BEGIN_NS

ALU::ALU(const Bus8* CU, Bus8* flag, Bus32* output, const Bus32* inputA, const Bus32* inputB) 
	: CU(CU), flag(flag), output(output), inputA(inputA), inputB(inputB) 
{

}

void ALU::process() {
	ALUInstrSet instr = static_cast<ALUInstrSet>(CU->extract_8());
	switch(instr) {
	case ALUInstrSet::Add32:
	{
		xdword a = static_cast<xdword>(inputA->extract_32());
		xdword b = static_cast<xdword>(inputB->extract_32());
		xdword res = a + b;
		setflags(!res, res > 0, res > dword_max, res > dword_max);
		output->bind_32(static_cast<dword>(res));
		break;
	}
	case ALUInstrSet::Sub32:
	{
		xdword a = static_cast<xdword>(inputA->extract_32());
		xdword b = static_cast<xdword>(inputB->extract_32());
		xdword res = a - b;
		setflags(!res, res > 0, res < dword_min, res >= dword_min);
		output->bind_32(static_cast<dword>(res));
		break;
	}
	case ALUInstrSet::Neg32:
	{
		dword a = inputA->extract_32();
		dword res = -a;
		setflags(!res, res > 0, a == dword_min, a == dword_min);
		output->bind_32(res);
		break;
	}
	case ALUInstrSet::Mul32:
	{
		xdword a = static_cast<int>(inputA->extract_32());
		xdword b = static_cast<int>(inputB->extract_32());
		xdword res = a * b;

		setflags(!res, res > 0, res > dword_max || res < dword_min, res > dword_max);

		output->bind_32(static_cast<dword>(res));
		break;
	}
	case ALUInstrSet::Div32:
	{
		dword a = inputA->extract_32();
		dword b = inputB->extract_32();
		if(b == 0) {
			setflags(0, (a > 0) ^ (b > 0), 1, 1);
			break;
		}
		dword res = a / b;
		setflags(!res, res > 0, 0, 0);
		output->bind_32(res);
		break;
	}
	case ALUInstrSet::Mod32:
	{
		dword a = inputA->extract_32();
		dword b = inputB->extract_32();
		if(b == 0) {
			setflags(0, (a > 0) ^ (b > 0), 1, 1);
			break;
		}
		dword res = a % b;
		setflags(!res, res > 0, 0, 0);
		output->bind_32(res);
		break;
	}
	case ALUInstrSet::Inc32:
	{
		dword a = inputA->extract_32();
		dword res = a + 1;
		setflags(!res, res > 0, a == dword_max, a == dword_max);
		output->bind_32(res);
		break;
	}
	case ALUInstrSet::Dec32:
	{
		dword a = inputA->extract_32();
		dword res = a - 1;
		setflags(!res, res > 0, a == dword_min, a != dword_min);
		output->bind_32(res);
		break;
	}
	case ALUInstrSet::LShift32:
	{
		dword a = inputA->extract_32();
		dword b = inputB->extract_32();
		dword res = a << b;
		bool ovf = b > 31 || (a & ((~0) << b));
		setflags(!res, res > 0, ovf, ovf);
		output->bind_32(res);
		break;
	}
	case ALUInstrSet::RShift32:
	{
		dword a = inputA->extract_32();
		dword b = inputB->extract_32();
		dword res = a >> b;
		bool ovf = b > 31 || (a & ~((~0) << b));
		setflags(!res, res > 0, ovf, !ovf);
		output->bind_32(res);
		break;
	}
	case ALUInstrSet::Comp32:
	{
		dword a = inputA->extract_32();
		dword b = inputB->extract_32();
		setflags(a == b, a > b, 0, 0);
		break;
	}
	case ALUInstrSet::Comp032:
	{
		dword a = inputA->extract_32();
		setflags(a == 0, a > 0, 0, 0);
		break;
	}
	case ALUInstrSet::Add16:
	{
		xdword a = static_cast<xdword>(inputA->extract_16());
		xdword b = static_cast<xdword>(inputB->extract_16());
		xdword res = a + b;
		setflags(!res, res > 0, res > word_max, res > word_max);
		output->bind_16(static_cast<word>(res));
		break;
	}
	case ALUInstrSet::Sub16:
	{
		dword a = static_cast<dword>(inputA->extract_16());
		dword b = static_cast<dword>(inputB->extract_16());
		dword res = a - b;
		setflags(!res, res > 0, res < word_min, res >= word_min);
		output->bind_16(static_cast<word>(res));
		break;
	}
	case ALUInstrSet::Neg16:
	{
		word a = inputA->extract_16();
		word res = -a;
		setflags(!res, res > 0, a == word_min, a == word_min);
		output->bind_16(res);
		break;
	}
	case ALUInstrSet::Mul16:
	{
		dword a = static_cast<int>(inputA->extract_16());
		dword b = static_cast<int>(inputB->extract_16());
		dword res = a * b;

		setflags(!res, res > 0, res > word_max || res < word_min, res > word_max);

		output->bind_16(static_cast<word>(res));
		break;
	}
	case ALUInstrSet::Div16:
	{
		word a = inputA->extract_16();
		word b = inputB->extract_16();
		if(b == 0) {
			setflags(0, (a > 0) ^ (b > 0), 1, 1);
			break;
		}
		word res = a / b;
		setflags(!res, res > 0, 0, 0);
		output->bind_16(res);
		break;
	}
	case ALUInstrSet::Mod16:
	{
		word a = inputA->extract_16();
		word b = inputB->extract_16();
		if(b == 0) {
			setflags(0, (a > 0) ^ (b > 0), 1, 1);
			break;
		}
		word res = a % b;
		setflags(!res, res > 0, 0, 0);
		output->bind_16(res);
		break;
	}
	case ALUInstrSet::Inc16:
	{
		word a = inputA->extract_16();
		word res = a + 1;
		setflags(!res, res > 0, a == word_max, a == word_max);
		output->bind_16(res);
		break;
	}
	case ALUInstrSet::Dec16:
	{
		word a = inputA->extract_16();
		word res = a - 1;
		setflags(!res, res > 0, a == word_min, a != word_min);
		output->bind_16(res);
		break;
	}
	case ALUInstrSet::LShift16:
	{
		word a = inputA->extract_16();
		word b = inputB->extract_16();
		word res = a << b;
		bool ovf = b > 31 || (a & ((~0) << b));
		setflags(!res, res > 0, ovf, ovf);
		output->bind_16(res);
		break;
	}
	case ALUInstrSet::RShift16:
	{
		word a = inputA->extract_16();
		word b = inputB->extract_16();
		word res = a >> b;
		bool ovf = b > 31 || (a & ~((~0) << b));
		setflags(!res, res > 0, ovf, !ovf);
		output->bind_16(res);
		break;
	}
	case ALUInstrSet::Comp16:
	{
		word a = inputA->extract_16();
		word b = inputB->extract_16();
		setflags(a == b, a > b, 0, 0);
		break;
	}
	case ALUInstrSet::Comp016:
	{
		word a = inputA->extract_16();
		setflags(a == 0, a > 0, 0, 0);
		break;
	}
	case ALUInstrSet::Add8:
	{
		word a = static_cast<word>(inputA->extract_8());
		word b = static_cast<word>(inputB->extract_8());
		word res = a + b;
		setflags(!res, res > 0, res > byte_max, res > byte_max);
		output->bind_8(static_cast<byte>(res));
		break;
	}
	case ALUInstrSet::Sub8:
	{
		word a = static_cast<word>(inputA->extract_8());
		word b = static_cast<word>(inputB->extract_8());
		word res = a - b;
		setflags(!res, res > 0, res < byte_min, res >= byte_min);
		output->bind_8(static_cast<byte>(res));
		break;
	}
	case ALUInstrSet::Neg8:
	{
		byte a = inputA->extract_8();
		byte res = -a;
		setflags(!res, res > 0, a == byte_min, a == byte_min);
		output->bind_8(res);
		break;
	}
	case ALUInstrSet::Mul8:
	{
		word a = static_cast<int>(inputA->extract_8());
		word b = static_cast<int>(inputB->extract_8());
		word res = a * b;

		setflags(!res, res > 0, res > byte_max || res < byte_min, res > byte_max);

		output->bind_8(static_cast<byte>(res));
		break;
	}
	case ALUInstrSet::Div8:
	{
		byte a = inputA->extract_8();
		byte b = inputB->extract_8();
		if(b == 0) {
			setflags(0, (a > 0) ^ (b > 0), 1, 1);
			break;
		}
		byte res = a / b;
		setflags(!res, res > 0, 0, 0);
		output->bind_8(res);
		break;
	}
	case ALUInstrSet::Mod8:
	{
		byte a = inputA->extract_8();
		byte b = inputB->extract_8();
		if(b == 0) {
			setflags(0, (a > 0) ^ (b > 0), 1, 1);
			break;
		}
		byte res = a % b;
		setflags(!res, res > 0, 0, 0);
		output->bind_8(res);
		break;
	}
	case ALUInstrSet::Inc8:
	{
		byte a = inputA->extract_8();
		byte res = a + 1;
		setflags(!res, res > 0, a == byte_max, a == byte_max);
		output->bind_8(res);
		break;
	}
	case ALUInstrSet::Dec8:
	{
		byte a = inputA->extract_8();
		byte res = a - 1;
		setflags(!res, res > 0, a == byte_min, a != byte_min);
		output->bind_8(res);
		break;
	}
	case ALUInstrSet::LShift8:
	{
		byte a = inputA->extract_8();
		byte b = inputB->extract_8();
		byte res = a << b;
		bool ovf = b > 31 || (a & ((~0) << b));
		setflags(!res, res > 0, ovf, ovf);
		output->bind_8(res);
		break;
	}
	case ALUInstrSet::RShift8:
	{
		byte a = inputA->extract_8();
		byte b = inputB->extract_8();
		byte res = a >> b;
		bool ovf = b > 31 || (a & ~((~0) << b));
		setflags(!res, res > 0, ovf, !ovf);
		output->bind_8(res);
		break;
	}
	case ALUInstrSet::Comp8:
	{
		byte a = inputA->extract_8();
		byte b = inputB->extract_8();
		setflags(a == b, a > b, 0, 0);
		break;
	}
	case ALUInstrSet::Comp08:
	{
		byte a = inputA->extract_8();
		setflags(a == 0, a > 0, 0, 0);
		break;
	}
	default:
		throw ALUInstrSetUnknown((dword)instr);
	};
}

void ALU::setflags(bool zero, bool comp, bool overflow, bool carry) {
	flag->bind_8(
		(zero		? bytev(ALUFlag::Zero)		: 0) |
		(comp		? bytev(ALUFlag::Comp)		: 0) |
		(overflow	? bytev(ALUFlag::Overflow)	: 0) |
		(carry		? bytev(ALUFlag::Carry)		: 0)
	);
}

END_NS