#include "stdafx.h"
#include "ALU.h"

#include <iostream>

BEGIN_NS

ALU::ALU(const Bus* CU, Bus* flag, Bus* output, const Bus* inputA, const Bus* inputB) 
	: CU(CU), flag(flag), output(output), inputA(inputA), inputB(inputB) 
{

}

void ALU::process() {
	switch(static_cast<ALU::CU_CODE>(CU->extract())) {
	case CU_CODE::ADD:
	{
		short a = static_cast<short>(inputA->extract());
		short b = static_cast<short>(inputB->extract());
		short r = a + b;
		setflag(ALU::FLAG_BIT::Overflow, r > byte_max);
		setflag(ALU::FLAG_BIT::Carry, r > byte_max);

		output->bind(static_cast<byte>(r));
		break;
	}
	case CU_CODE::SUB:
	{
		short a = static_cast<short>(inputA->extract());
		short b = static_cast<short>(inputB->extract());
		short r = a - b;
		setflag(ALU::FLAG_BIT::Overflow, r < byte_min);
		setflag(ALU::FLAG_BIT::Carry, r >= byte_min);

		output->bind(static_cast<byte>(r));
		break;
	}
	case CU_CODE::NEG:
	{
		byte a = inputA->extract();
		setflag(FLAG_BIT::Overflow, a == byte_min);
		setflag(FLAG_BIT::Carry, a == byte_min);
		output->bind(-a);
		break;
	}
	case CU_CODE::MUL:
	{
		int a = static_cast<int>(inputA->extract());
		int b = static_cast<int>(inputB->extract());
		int r = a * b;

		setflag(ALU::FLAG_BIT::Overflow, r > byte_max || r < byte_min);
		setflag(ALU::FLAG_BIT::Carry, r > byte_max);

		output->bind(static_cast<byte>(r));
		break;
	}
	case CU_CODE::DIV:
	{
		byte b = inputB->extract();
		if(b == 0) {
			setflag(ALU::FLAG_BIT::Overflow, 1);
			setflag(ALU::FLAG_BIT::Carry, 1);
			break;
		}

		setflag(ALU::FLAG_BIT::Overflow, 0);
		setflag(ALU::FLAG_BIT::Carry, 0);
		output->bind(inputA->extract() / b);
		break;
	}
	case CU_CODE::MOD:
	{
		byte b = inputB->extract();
		if(b == 0) {
			setflag(ALU::FLAG_BIT::Overflow, 1);
			setflag(ALU::FLAG_BIT::Carry, 1);
			break;
		}

		setflag(ALU::FLAG_BIT::Overflow, 0);
		setflag(ALU::FLAG_BIT::Carry, 0);
		output->bind(inputA->extract() % b);
		break;
	}
	case CU_CODE::INC:
	{
		byte a = inputA->extract();
		setflag(ALU::FLAG_BIT::Overflow, a == byte_max);
		setflag(ALU::FLAG_BIT::Carry, a == byte_max);
		output->bind(a + 1);
		break;
	}
	case CU_CODE::DEC:
	{
		byte a = inputA->extract();
		setflag(ALU::FLAG_BIT::Overflow, a == byte_min);
		setflag(ALU::FLAG_BIT::Carry, a != byte_min);
		output->bind(a - 1);
		break;
	}
	case CU_CODE::LSHFT:
	{
		byte a = inputA->extract();
		bool ovf = a & (1 << 8);
		setflag(ALU::FLAG_BIT::Overflow, ovf);
		setflag(ALU::FLAG_BIT::Carry, ovf);
		output->bind(a << 1);
		break;
	}
	case CU_CODE::RSHFT:
	{
		byte a = inputA->extract();
		bool ovf = a & 1;
		setflag(ALU::FLAG_BIT::Overflow, ovf);
		setflag(ALU::FLAG_BIT::Carry, !ovf);
		output->bind(a >> 1);
		break;
	}
	case CU_CODE::VLSHFT:
	{
		byte a = inputA->extract();
		byte b = inputB->extract();
		bool ovf = b > 8 || (a & ((~0) << b));
		setflag(ALU::FLAG_BIT::Overflow, ovf);
		setflag(ALU::FLAG_BIT::Carry, ovf);
		output->bind(a << b);
		break;
	}
	case CU_CODE::VRSHFT:
	{
		byte a = inputA->extract();
		byte b = inputB->extract();
		bool ovf = b > 8 || (a & ~((~0) << b));
		setflag(ALU::FLAG_BIT::Overflow, ovf);
		setflag(ALU::FLAG_BIT::Carry, !ovf);
		output->bind(a >> b);
		break;
	}
	case CU_CODE::COMP:
	{
		byte res = inputA->extract() - inputB->extract();
		setflag(ALU::FLAG_BIT::Comp, res > 0);
		setflag(ALU::FLAG_BIT::Zero, res == 0);
		break;
	}
	case CU_CODE::COMP0:
	{
		byte res = inputA->extract();
		setflag(ALU::FLAG_BIT::Comp, res > 0);
		setflag(ALU::FLAG_BIT::Zero, res == 0);
		break;
	}
	default:
		exit(1);
	};
}

ubyte ALU::bit(ALU::FLAG_BIT b) const {
	return static_cast<ubyte>(b);
}

void ALU::setflag(FLAG_BIT f, ubyte v) {
	flag->bit(bit(f)) = v;
}

END_NS