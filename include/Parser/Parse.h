#pragma once

#include "IR/ir.hpp"
#include <string>

namespace Parser {
/**
* @brief: Parse the input as C++ code and return the intermediate representation.
*
* @param: std::string const& code
*
* @return: IR::Namespace
*/
IR::Namespace parseString(std::string const& code);
}
