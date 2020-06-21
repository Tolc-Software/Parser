#pragma once

#include "Parser/Config.hpp"

namespace Helpers {
/**
* Sets up the global logger
* Sets the log level for all loggers
*/
void setupLogging(Parser::Config::LogLevel level);
}
