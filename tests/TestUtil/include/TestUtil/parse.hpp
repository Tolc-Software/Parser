#pragma once

#include "Parser/Parse.hpp"
#include <IR/ir.hpp>
#include <catch2/catch.hpp>
#include <string>

namespace TestUtil {

IR::Namespace parseString(std::string const& code) {
	return Parser::parseString(code);
}
}    // namespace TestUtil
