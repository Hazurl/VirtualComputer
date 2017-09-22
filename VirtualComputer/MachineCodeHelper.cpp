#include "stdafx.h"
#include "MachineCodeHelper.h"

BEGIN_NS

MachineCode MachineCodeHelper::generate_fibonacci() {
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