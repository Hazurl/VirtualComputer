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
	while (p < text.size()) {
		char c = text[p]; 
		if (c == '\n') {
			if (is.tokens.size() > 0)
				return is;
			p++;
		} 
		else
			is.tokens.push_back(nextToken());
	}
	return is;
}

std::string Compiler::nextToken() {
	std::string s = "";
	char c = text[p++];
	if (c == '.' || c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || c == '-') {
		s += c;
		while (p < text.size() 
			&& (c = text[p++], 
			(c >= 'a' && c <= 'z' || 
			 c >= 'A' && c <= 'Z' || 
			 c == '_' || c == '-' || 
			 c >= '0' && c <= '9' || c == '.')))
			s += c;
		return s;
	}
	if (c == '"') {
		s += c;
		while (p < text.size() && (c = text[p++], c != '"'))
			s += c;
		return s += c;
	}
	if (c == ',')
		return s += c;
	if (c == ':')
		return s += c;

	throw 42;
}

END_NS