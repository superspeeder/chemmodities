#include "Utils.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

std::string chengine::readFile(const char* path) {
	std::ifstream t(path);
	std::stringstream buffer;
	buffer << t.rdbuf();

	return buffer.str();
}

chengine::Directive chengine::parseDirective(const char* line) {
	std::string type;
	std::string value;


	int p = 0;

	for (int i = 1; i < strlen(line); i++) {
		char c = line[i];
		if (p == 0) {
			if (c == ' ') {
				p = 1;
			}
			else {
				type.push_back(c);
			}
		}
		else {
			value.push_back(c);
		}
	}
	return Directive{ type,value };
}

bool chengine::lineContainsDirective(const char* line) {
	return line[0] == '#';
}