#pragma once

#include <iostream>
#include "config.h"
#include "MachineCode.h"
#include "Ram.h"

BEGIN_NS

class MachineCodeHelper {
public:

	template<ubyte linker_size, unsigned int ram_size>
	static void load_to_ram(Ram<ram_size>* ram, MachineCode* code, dword(&linker)[linker_size], udword& seg_instr, udword& seg_stack, dword size_stack = 128) {
		auto _code = code->generate(linker);

		seg_instr = ram->reserve(_code.size() * sizeof(dword));
		seg_stack = ram->reserve(size_stack);

		for(ubyte i = 0; i < _code.size(); ++i) {
			ram->load_at(seg_instr + i * 4, _code[i]);
		}

		/*
		std::cout << "Lookup RAM at " << seg_instr <<  " (" << _code.size() << ") :" << std::endl;
		for(ubyte i = 0; i < _code.size(); ++i) {
			std::cout << seg_instr + i * 4 << " : " << ram->lookup32(seg_instr + i * 4) << std::endl;
		}
		std::cin.get();*/
	}

	static MachineCode generate_fibonacci();
	static MachineCode generate_counter();
	static MachineCode generate_simple_counter();
	static MachineCode generate_for_in_range();

};

END_NS
