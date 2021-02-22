#pragma once

#include "Parser/Parse.hpp"
#include <IR/ir.hpp>
#include <catch2/catch.hpp>
#include <filesystem>
#include <optional>
#include <spdlog/spdlog.h>
#include <string>

namespace TestUtil {

IR::Namespace
parseString(std::string const& code,
            spdlog::level::level_enum logLevel = spdlog::level::warn) {
	spdlog::set_level(logLevel);
	auto ns = Parser::parseString(code);
	REQUIRE(ns.has_value());
	return ns.has_value() ? ns.value() : IR::Namespace();
}

IR::Namespace
parseFile(std::filesystem::path const& filepath,
          spdlog::level::level_enum logLevel = spdlog::level::warn) {
	spdlog::set_level(logLevel);
	auto ns = Parser::parseFile(filepath);
	REQUIRE(ns.has_value());
	return ns.has_value() ? ns.value() : IR::Namespace();
}
}    // namespace TestUtil
