#pragma once

#include "config.h"

BEGIN_NS

enum class ALUInstrSet : ubyte {
	Add,		// 2 arg : a + b
	Sub,		// 2 arg : a - b
	Mul,		// 2 arg : a * b
	Div,		// 2 arg : a / b
	Mod,		// 2 arg : a % b
	Neg,		// 1 arg : -a
	Inc,		// 1 arg : a + 1 
	Dec,		// 1 arg : a - 1
	LShift,		// 1 arg : a << 1
	RShift,		// 1 arg : a >> 1
	VLShift,	// 2 arg : a << b
	VRShift,	// 2 arg : a >> b
	Comp,		// 2 arg : a ? b
	Comp0,		// 1 arg : a ? 0
};

enum class InstrSet : ubyte {
	HALT = 0,

	// Operation on flag
	Jmp,
	Jmpz,		// args: addr_to_jmp
	Jmpnz,		// args: addr_to_jmp
	Jmpg,		// args: addr_to_jmp
	Jmpgz,		// args: addr_to_jmp
	Jmpl,		// args: addr_to_jmp
	Jmplz,		// args: addr_to_jmp

	// Operation on register A and B
	Add_ab,
	Sub_ab,
	Mul_ab,
	Div_ab,
	Mod_ab,
	LShift_ab,
	RShift_ab,
	Comp_ab,
	Move_ab,
	Move_ba,

	// Operation on register a
	Inc_a,
	Dec_a,
	Neg_a,
	LShift_a,
	RShift_a,
	Comp0_a,
	Push_a,
	Pop_a,
	Load_a,		// args: value
	LoadRel_a,	// args: addr_value
	Store_a,	// args: addr
	Out_a,

	// Operation on register b
	Inc_b,
	Dec_b,
	Neg_b,
	LShift_b,
	RShift_b,
	Comp0_b,
	Push_b,
	Pop_b,
	Load_b,		// args: value
	LoadRel_b,	// args: addr_value
	Store_b,	// args: addr
	Out_b,

	// Operation on register c
	Inc_c,
	Dec_c,
	Neg_c,
	LShift_c,
	RShift_c,
	Comp0_c,
	Push_c,
	Pop_c,
	Load_c,		// args: value
	LoadRel_c,	// args: addr_value
	Store_c,	// args: addr
	Out_c,
};

inline byte bytev(InstrSet s) {
	return static_cast<byte>(s);
}

inline byte bytev(ALUInstrSet s) {
	return static_cast<byte>(s);
}

END_NS