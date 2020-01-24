#pragma once

#include "IR/ir.hpp"
#include <string>

namespace Parser {
/**
* @brief: Parse the input as C++ code and return the intermediate representation.
*
* @param: const std::string& code
*
* @return: IR::Namespace
*/
IR::Namespace parseString(const std::string& code);
}
