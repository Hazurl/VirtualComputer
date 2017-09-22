#pragma once

#include "config.h"
#include "MachineCode.h"
#include "Ram.h"

BEGIN_NS

class MachineCodeHelper {
public:

	template<ubyte linker_size, unsigned int ram_size>
	static void load_to_ram(Ram<ram_size>* ram, MachineCode* code, byte(&linker)[linker_size], ubyte& seg_instr, ubyte& seg_stack) {
		auto _code = code->generate(linker);

		seg_instr = ram->reserve(_code.size());
		seg_stack = ram->reserve(1);

		for(ubyte i = 0; i < _code.size(); ++i) {
			ram->load_at(seg_instr + i, _code[i]);
		}
	}

	static MachineCode generate_fibonacci();

};

END_NS