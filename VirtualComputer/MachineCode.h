#pragma once

#include "config.h"
#include "CU.h"
#include "InstrSet.h"

#include <vector>
#include <unordered_map>

BEGIN_NS

class MachineCode {
public:

	struct Generated {
		std::vector<dword> code;
		std::vector<byte> data;
		udword bss_size;
		udword stack_size;
		udword entry_point;
	};

	Generated generate();

	void command(dword cmd);
	void command(InstrSet cmd);
	void command(InstrSet cmd, InstrTarget t0);
	void command(InstrSet cmd, InstrTarget t0, InstrTarget t1);
	void label(byte id);
	void go_to(byte label);

	void request_stack(udword s);
	void request_bss(udword s);

	void add_data(std::vector<byte> d);
	void add_data_8(byte b);
	void add_data_16(word b);
	void add_data_32(dword b);

private:
	struct Instr {
		enum class State : byte {
			cmd,
			label,
			label_addr
		};
		State state;
		dword cmd;

		Instr(State s, dword b);
	};

	std::vector<Instr> code;
	std::vector<byte> data;
	udword wanted_bss_size;
	udword wanted_stack_size = 256;
};

END_NS