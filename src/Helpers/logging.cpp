#include "Helpers/logging.hpp"
#include "Parser/Config.hpp"
#include <spdlog/spdlog.h>

namespace Helpers {

void setLogLevel(Parser::Config::LogLevel level) {
	using Level = Parser::Config::LogLevel;
	switch (level) {
		case Level::Debug: spdlog::set_level(spdlog::level::debug); break;
		case Level::Warning: spdlog::set_level(spdlog::level::warn); break;
		case Level::Error: spdlog::set_level(spdlog::level::err); break;
	}
}

void setupLogging(Parser::Config::LogLevel level) {
	setLogLevel(level);

	// TODO: Use a custom pattern with set_pattern
	//       https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
}
}    // namespace Helpers
