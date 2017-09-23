#pragma once

#include "config.h"

BEGIN_NS

enum class ALUFlag : byte {
	Zero		= 1 << 0, 
	Comp		= 1 << 1, 
	Overflow	= 1 << 2, 
	Carry		= 1 << 3,
};

enum class ALUInstrSet : ubyte {
	Add8,		// 2 arg : a + b
	Sub8,		// 2 arg : a - b
	Mul8,		// 2 arg : a * b
	Div8,		// 2 arg : a / b
	Mod8,		// 2 arg : a % b
	Neg8,		// 1 arg : -a
	Inc8,		// 1 arg : a + 1 
	Dec8,		// 1 arg : a - 1
	LShift8,	// 1 arg : a << 1
	RShift8,	// 1 arg : a >> 1
	VLShift8,	// 2 arg : a << b
	VRShift8,	// 2 arg : a >> b
	Comp8,		// 2 arg : a ? b
	Comp08,		// 1 arg : a ? 0

	Add16,		// 2 arg : a + b
	Sub16,		// 2 arg : a - b
	Mul16,		// 2 arg : a * b
	Div16,		// 2 arg : a / b
	Mod16,		// 2 arg : a % b
	Neg16,		// 1 arg : -a
	Inc16,		// 1 arg : a + 1 
	Dec16,		// 1 arg : a - 1
	LShift16,	// 1 arg : a << 1
	RShift16,	// 1 arg : a >> 1
	VLShift16,	// 2 arg : a << b
	VRShift16,	// 2 arg : a >> b
	Comp16,		// 2 arg : a ? b
	Comp016,	// 1 arg : a ? 0

	Add32,		// 2 arg : a + b
	Sub32,		// 2 arg : a - b
	Mul32,		// 2 arg : a * b
	Div32,		// 2 arg : a / b
	Mod32,		// 2 arg : a % b
	Neg32,		// 1 arg : -a
	Inc32,		// 1 arg : a + 1 
	Dec32,		// 1 arg : a - 1
	LShift32,	// 1 arg : a << 1
	RShift32,	// 1 arg : a >> 1
	VLShift32,	// 2 arg : a << b
	VRShift32,	// 2 arg : a >> b
	Comp32,		// 2 arg : a ? b
	Comp032,	// 1 arg : a ? 0
};

enum class InstrSet : ubyte {
	Halt = 0,

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

inline dword dwordv(InstrSet s) {
	return static_cast<dword>(s);
}

inline dword dwordv(ALUInstrSet s) {
	return static_cast<dword>(s);
}

inline dword dwordv(ALUFlag s) {
	return static_cast<dword>(s);
}

inline word wordv(InstrSet s) {
	return static_cast<word>(s);
}

inline word wordv(ALUInstrSet s) {
	return static_cast<word>(s);
}

inline word wordv(ALUFlag s) {
	return static_cast<word>(s);
}

inline byte bytev(InstrSet s) {
	return static_cast<byte>(s);
}

inline byte bytev(ALUInstrSet s) {
	return static_cast<byte>(s);
}

inline byte bytev(ALUFlag s) {
	return static_cast<byte>(s);
}

END_NS