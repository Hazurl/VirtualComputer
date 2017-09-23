#pragma once

#include "config.h"
#include "CU.h"
#include "InstrSet.h"

#include <vector>
#include <unordered_map>

BEGIN_NS

class MachineCode {
public:

	std::vector<dword> generate(dword* linker);

	void command(dword cmd);
	void command(InstrSet cmd);
	void link(byte id);
	void label(byte id);
	void go_to(byte label);

private:
	struct Instr {
		enum class State : byte {
			cmd,
			link,
			label,
			label_addr
		};
		State state;
		dword cmd;

		Instr(State s, dword b);
	};

	std::vector<Instr> code;

};

END_NS