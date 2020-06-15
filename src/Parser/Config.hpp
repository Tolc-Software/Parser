#pragma once

#include <string>
#include <vector>

namespace Parser {
/**
* The values in this struct corresponds to the default values in configuring what Parser should do.
* There should be no functions or logic here, just values.
*/
struct Config {
	// Absolute paths to system headers
	std::vector<std::string> m_systemIncludes {};
};
}
