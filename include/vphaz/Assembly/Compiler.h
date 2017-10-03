#pragma once

#include <vphaz/config.h>
#include <vphaz/MachineCode/MachineCode.h>
#include <vphaz/Assembly/Parser.h>
#include <vphaz/Assembly/Analyzer.h>

#include <string>

BEGIN_NS_ASS

class Compiler {
public:
	Compiler(std::string const& text);

	MachineCode::Generated process();

private:

	Parser parser;
	Analyzer analyzer;
};

END_NS_ASS