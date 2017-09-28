#pragma once

#include <iostream>
#include <vphaz/config.h>
#include <vphaz/MachineCode/MachineCode.h>
#include <vphaz/Hardware/Memory/Ram.h>

BEGIN_NS

class MachineCodeHelper {
public:

	template<unsigned int ram_size>
	static void load_to_ram(Ram<ram_size>* ram, MachineCode* code, udword& seg_instr, udword& seg_data, udword& seg_bss, udword& seg_stack, udword& entry_point) {
		auto gen = code->generate();

		udword size_code = gen.code.size() * sizeof(typename decltype(gen.code)::value_type);
		udword size_data = gen.data.size() * sizeof(typename decltype(gen.data)::value_type);
		udword size = size_code + size_data + gen.bss_size + gen.stack_size;

		seg_instr = ram->reserve(size);
		seg_data = size_code + seg_instr;
		seg_bss = size_data + seg_data;
		seg_stack = gen.bss_size + seg_bss;

		entry_point = seg_instr + gen.entry_point;

		for(ubyte i = 0; i < gen.code.size(); ++i) {
			ram->load_at(seg_instr + i * 4, gen.code[i]);
		}

		for(ubyte i = 0; i < gen.data.size(); ++i) {
			ram->load_at(seg_data + i, gen.data[i]);
		}

		for(ubyte i = 0; i < gen.bss_size; ++i) {
			ram->load_at(seg_bss + i, (byte)0);
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
	static MachineCode generate_hello_world();

};

END_NS
