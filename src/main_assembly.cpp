#include <iostream>
#include <fstream>
#include <string>

#include <vphaz/Assembly/Compiler.h>

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cout << "Path needed in arguments" << std::endl;
		return 1;
	}

	std::string path = argv[1];
	std::ifstream file(path);
	std::string text = "";

	if (!file) {
		std::cout << "File not found: " << path << std::endl;
		return 1;
	}

	while (file.good()) {
		std::string line;
		std::getline(file, line);
		text += line + '\n';
	}

	using namespace NAMESPACE::NAMESPACE_ASS;

	Compiler c(text);

	c.process();

    return 0;
}

