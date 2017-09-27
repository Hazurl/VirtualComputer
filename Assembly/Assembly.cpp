// Assembly.cpp : Définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Compiler.h"

int main()
{
	std::cout << "Enter filepath: " << std::endl;
	std::string path;
	std::cin >> path;

	std::ifstream file(path);
	std::string text = "";

	if (!file) {
		std::cout << "File not found" << std::endl;
		return 1;
	}

	while (file.good()) {
		std::string line;
		std::getline(file, line);
		text += line + '\n';
	}

	using namespace NAMESPACE;

	Compiler c(text);

	c.process();

	std::cin.ignore();
	std::cin.get();

    return 0;
}

