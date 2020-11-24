#pragma once

#include <string>
#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace che {

	class Application;

	struct Directive {
		std::string type;
		std::string value;
	};

	std::string readFile(const char* path);

	Directive parseDirective(const char* line);

	bool lineContainsDirective(const char* line);

	class Logging {
	public:
		std::shared_ptr<spdlog::logger> logger;

		Logging();
	};

	class Disposable {
	public:
		static che::Application* s_mainDisposer;
		static bool s_autoDisposable;

		virtual ~Disposable() {};

		static inline void setAutoDisposer(che::Application* disposer) {
			s_mainDisposer = disposer;
			s_autoDisposable = true;
		};

	};
}