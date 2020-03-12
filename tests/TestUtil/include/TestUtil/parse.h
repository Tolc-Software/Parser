#pragma once

#include "Parser/Parse.h"
#include <IR/ir.hpp>
#include <catch2/catch.hpp>
#include <string>

namespace TestUtil {

IR::Namespace parseString(std::string const& code) {
	UNSCOPED_INFO(code);
	return Parser::parseString(code);
}
}    // namespace TestUtil
