#pragma once

#include <string>

namespace chengine {

	struct Directive {
		std::string type;
		std::string value;
	};

	std::string readFile(const char* path);

	Directive parseDirective(const char* line);

	bool lineContainsDirective(const char* line);

}