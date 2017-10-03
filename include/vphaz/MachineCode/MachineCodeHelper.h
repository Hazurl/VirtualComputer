#pragma once

#include <iostream>
#include <vphaz/config.h>
#include <vphaz/MachineCode/MachineCode.h>
#include <vphaz/Hardware/Memory/Ram.h>

BEGIN_NS

class MachineCodeHelper {
public:

	template<unsigned int ram_size>
	static void load_to_ram(Ram<ram_size>* ram, MachineCode* code, udword& seg_instr, udword& seg_stack, udword& entry_point) {
		auto gen = code->generate();

		udword size_code = gen.code.size() * sizeof(typename decltype(gen.code)::value_type);
		udword size_data = gen.data.size() * sizeof(typename decltype(gen.data)::value_type);
		udword size = size_code + size_data + gen.bss_size + gen.stack_size;

		// at 0 : BSS
		// at <bss_size> : DATA
		// at <bss_size + data_size> : CODE
		// at <bss_size + data_size + code_size> : STACK

		seg_instr = ram->reserve(size);
		seg_stack = gen.bss_size;

		entry_point = seg_instr + gen.entry_point;

		for(ubyte i = 0; i < gen.bss_size; ++i) {
			ram->load_at(seg_instr + i, (byte)0);
		}

		for(ubyte i = 0; i < gen.data.size(); ++i) {
			ram->load_at(seg_instr + gen.bss_size + i, gen.data[i]);
		}

		for(ubyte i = 0; i < gen.code.size(); ++i) {
			ram->load_at(seg_instr + gen.bss_size + size_data + i * 4, gen.code[i]);
		}
/*
		std::cout << "RAM lookup !" << std::endl;
		std::cout << "  BSS" << std::endl;
		auto offset = seg_instr;
		for (int i = 0; i < gen.bss_size; ++i)
			std::cout << offset << " : " << (int)ram->lookup8(offset + i) << std::endl;

		offset += gen.bss_size;
		std::cout << "  Data" << std::endl;
		for (int i = 0; i < gen.data.size(); ++i)
			std::cout << i + offset << " : " << (int)ram->lookup8(offset + i) << std::endl;

		std::cout << "  Code" << std::endl;
		offset += gen.data.size();
		for (int i = 0; i < gen.code.size(); ++i)
			std::cout << offset + i * 4 << " : " << ram->lookup16(offset + i * 4 + 2) << "-" << (int)ram->lookup8(offset + i * 4 + 1) << "-" << (int)ram->lookup8(offset + i * 4) 
			<< " (" << ram->lookup32(offset + i * 4) << ")" << std::endl;

		std::cin.get();
*/	}

	static MachineCode generate_fibonacci();
	static MachineCode generate_counter();
	static MachineCode generate_simple_counter();
	static MachineCode generate_for_in_range();
	static MachineCode generate_hello_world();

};

END_NS
