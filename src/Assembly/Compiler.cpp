#include <vphaz/Assembly/Compiler.h>

#include <iostream>

BEGIN_NS_ASS

Compiler::Compiler(std::string const& text) : parser(text) {

}

MachineCode::Generated Compiler::process() {
	parser.tokenize();

	if (!parser.good()) {
		std::cout << "Parser Error: " << parser.getError() << std::endl;
		return {};
	}

	return analyzer.analyze(parser.getTokens());
}

END_NS_ASS