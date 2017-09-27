#pragma once

#include "../VirtualComputer/config.h"
#include "../VirtualComputer/MachineCode.h"
#include <string>

BEGIN_NS

class Compiler {
public:
	Compiler(std::string const& text);

	MachineCode::Generated process();

private:

	struct Instruction {
		std::vector<std::string> tokens;
	};

	Instruction nextInstr();
	std::string nextToken();

	std::string text;
	unsigned int p = 0;

};

END_NS