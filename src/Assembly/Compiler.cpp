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

	auto code = analyzer.analyze(parser.getTokens());
	if (!analyzer.good()) {
		std::cout << "Analyzer Error: " << analyzer.getError() << std::endl;
		return {};
	}
	return code;
}

END_NS_ASS