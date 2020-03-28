#pragma once

#include "IR/ir.hpp"
#include <string>

namespace Parser {
/**
* @brief: Parse the input file as C++ code and return the intermediate representation.
*
* @param: std::string const& filename
*
* @return: IR::Namespace - The global namespace
*/
IR::Namespace parseFile(std::string const& filename);

/**
* @brief: Parse the input as C++ code and return the intermediate representation.
*
* @param: std::string const& code
*
* @return: IR::Namespace - The global namespace
*/
IR::Namespace parseString(std::string const& code);
}
