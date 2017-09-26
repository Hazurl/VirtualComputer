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
	LShift8,	// 1 arg : a << b
	RShift8,	// 1 arg : a >> b
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
	LShift16,	// 1 arg : a << b
	RShift16,	// 1 arg : a >> b
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
	LShift32,	// 1 arg : a << b
	RShift32,	// 1 arg : a >> b
	Comp32,		// 2 arg : a ? b
	Comp032,	// 1 arg : a ? 0
};

enum class InstrSet : uword {
	Halt = 0,

	// Operation on flag
	Jmp,
	Jmpz,		// args: addr_to_jmp
	Jmpnz,		// args: addr_to_jmp
	Jmpg,		// args: addr_to_jmp
	Jmpgz,		// args: addr_to_jmp
	Jmpl,		// args: addr_to_jmp
	Jmplz,		// args: addr_to_jmp

	DeferData, DeferBss,
	Comp, Comp0,
	Add, Sub,
	Mul, Div, Mod,
	Inc, Dec,
	Neg, 
	LShift, RShift,
	Push, Pop,
	Load, Store,
	Out
};

enum class InstrTarget : ubyte {
	a32, b32, c32,
	a16, b16, c16,
	a16h, b16h, c16h,
	a8, b8, c8,
	a8hh, b8hh, c8hh,
	a8lh, b8lh, c8lh,
	a8hl, b8hl, c8hl,

	AddrData, AddrBss, AddrInstr,
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

inline byte bytev(ALUInstrSet s) {
	return static_cast<byte>(s);
}

inline byte bytev(ALUFlag s) {
	return static_cast<byte>(s);
}

END_NS