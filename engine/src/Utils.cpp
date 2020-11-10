#include "Utils.hpp"
#define WINDOWS

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>  // defines FILENAME_MAX
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif


std::string get_current_dir() {
	char buff[FILENAME_MAX]; //create string buffer to hold path
	GetCurrentDir(buff, FILENAME_MAX);
	std::string current_working_dir(buff);
	return current_working_dir;
}

std::string chengine::readFile(const char* path) {
	std::ifstream t(path);
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::cout << get_current_dir() << "\n";

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