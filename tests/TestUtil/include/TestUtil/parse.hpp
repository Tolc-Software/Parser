#pragma once

#include "Parser/Parse.hpp"
#include <IR/ir.hpp>
#include <catch2/catch.hpp>
#include <optional>
#include <string>

namespace TestUtil {

IR::Namespace parseString(std::string const& code) {
	auto ns = Parser::parseString(code);
	REQUIRE(ns.has_value());
	return ns.has_value() ? ns.value() : IR::Namespace();
}

IR::Namespace parseFile(std::string const& filepath) {
	auto ns = Parser::parseFile(filepath);
	REQUIRE(ns.has_value());
	return ns.has_value() ? ns.value() : IR::Namespace();
}
}    // namespace TestUtil
