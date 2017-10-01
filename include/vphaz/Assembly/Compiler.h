#pragma once

#include <vphaz/config.h>
#include <vphaz/MachineCode/MachineCode.h>
#include <vphaz/Assembly/Parser.h>
#include <vphaz/Assembly/Analyzer.h>

#include <string>

BEGIN_NS

class Compiler {
public:
	Compiler(std::string const& text);

	MachineCode::Generated process();

private:

	Parser parser;
	Analyzer analyzer;
};

END_NS