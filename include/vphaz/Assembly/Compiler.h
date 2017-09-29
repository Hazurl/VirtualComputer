#pragma once

#include <vphaz/config.h>
#include <vphaz/MachineCode/MachineCode.h>
#include <vphaz/Assembly/Parser.h>

#include <string>

BEGIN_NS

class Compiler {
public:
	Compiler(std::string const& text);

	MachineCode::Generated process();

private:

	Parser parser;
};

END_NS