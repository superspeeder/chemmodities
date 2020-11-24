#include "Utils/Utils.hpp"
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

std::string che::readFile(const char* path) {
	std::ifstream t(path);
	std::stringstream buffer;
	buffer << t.rdbuf();

	return buffer.str();
}

che::Directive che::parseDirective(const char* line) {
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

bool che::lineContainsDirective(const char* line) {
	return line[0] == '#';
}

che::Logging::Logging() {
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::debug);
    
	auto max_size = 1048576 * 5;
    auto max_files = 3;
    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/latest.txt", max_size, max_files);
    file_sink->set_level(spdlog::level::trace);

	spdlog::sinks_init_list sinks = {console_sink, file_sink};

	logger = std::make_shared<spdlog::logger>("logger", sinks);

	spdlog::set_default_logger(logger);
	spdlog::info("Initialized logger");
	spdlog::set_level(spdlog::level::level_enum::trace);
}

bool che::Disposable::s_autoDisposable = false;
che::Application* che::Disposable::s_mainDisposer = nullptr;