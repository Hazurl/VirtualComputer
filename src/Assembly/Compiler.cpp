#include <vphaz/Assembly/Compiler.h>

#include <iostream>

BEGIN_NS

Compiler::Compiler(std::string const& text) : parser(text) {

}

MachineCode::Generated Compiler::process() {
	parser.tokenize();

	return analyzer.analyze(parser.getTokens());
}

END_NS