#include "stdafx.h"
#include "Compiler.h"
#include <iostream>

BEGIN_NS

Compiler::Compiler(std::string const& text) : text(text) {

}

MachineCode::Generated Compiler::process() {
	std::vector<Compiler::Instruction> iss;
	while (p < text.size())
		iss.push_back(nextInstr());

	for (auto& is : iss) {
		for (auto& s : is.tokens) {
			std::cout << s << "//";
		}
		std::cout << std::endl;
	}

	return {};
}

Compiler::Instruction Compiler::nextInstr() {
	Compiler::Instruction is;
	char c;
	std::string cur = "";
	while (p < text.size() && (c = text[p++]) != '\n' && is.tokens.size()) {
		if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || c == '-') {
			cur += c;
		}
		else {
			if (cur != "")
				is.tokens.push_back(cur);
			cur = "";
		}
	}
	if (cur != "")
		is.tokens.push_back(cur);
	return is;
}

END_NS