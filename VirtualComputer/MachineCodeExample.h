#pragma once

#include "config.h"
#include "MachineCode.h"
#include "Ram.h"

BEGIN_NS

template<ubyte linker_size, unsigned int ram_size>
void load_to_ram(Ram<ram_size>* ram, MachineCode* code, byte(&linker)[linker_size], ubyte& seg_instr, ubyte& seg_stack) {
	auto _code = code->generate(linker);

	seg_instr = ram->reserve(_code.size());
	seg_stack = ram->reserve(1);

	for(ubyte i = 0; i < _code.size(); ++i) {
		ram->load_at(seg_instr + i, _code[i]);
	}
}

MachineCode generate_fibonacci() {
	MachineCode fibonacci;
	fibonacci.command(CU::OP::LOADA);
	fibonacci.command(1);
	fibonacci.command(CU::OP::LOADB);
	fibonacci.command(1);
	fibonacci.command(CU::OP::LOADC);
	fibonacci.link(0);

	fibonacci.label(0);
	fibonacci.command(CU::OP::COMPC0);
	fibonacci.command(CU::OP::JMPLE);
	fibonacci.go_to(1);

	fibonacci.command(CU::OP::OUTA);
	fibonacci.command(CU::OP::PUSHA);
	fibonacci.command(CU::OP::ADD);
	fibonacci.command(CU::OP::POPB);
	fibonacci.command(CU::OP::DECCOUNTER);

	fibonacci.command(CU::OP::JMP);
	fibonacci.go_to(0);

	fibonacci.label(1);
	fibonacci.command(CU::OP::OUTA);
	fibonacci.command(CU::OP::END);
	return fibonacci;
}

END_NS