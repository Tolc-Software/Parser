#include "Parser/Parse.hpp"
#include "Parser/Config.hpp"
#include "TestUtil/files.hpp"
#include <catch2/catch.hpp>
#include <filesystem>
#include <string>

TEST_CASE("Incorrect C++ does not emit a namespace", "[Parse]") {
	auto code = R"(
class MyClass {
	// Using string without including the header
	std::string s;
};
)";
	CAPTURE(code);
	auto ns = Parser::parseString(code);
	REQUIRE(!ns.has_value());

	auto file = TestUtil::writeToFile(code);
	auto nsFromFile = Parser::parseFile(file);
	REQUIRE(!nsFromFile.has_value());
}

TEST_CASE("C++ code within a file does not depend on the file extension",
          "[Parse]") {
	for (auto extension :
	     {".h", ".c", ".txt", "", ".cc", ".cpp", ".hpp", ".cxx", ".py"}) {
		auto code = R"(
class MyClass {
	int i;
};
)";
		CAPTURE(extension);
		auto ns = Parser::parseString(code);
		REQUIRE(ns.has_value());

		auto file = TestUtil::writeToFile(code, extension);
		auto nsFromFile = Parser::parseFile(file);
		REQUIRE(nsFromFile.has_value());
	}
}

TEST_CASE("Without paths to system includes, code including std headers fail",
          "[Parse]") {
	auto code = R"(
#include <string>

class MyClass {
	// Using string without including the header
	std::string s;
};
)";

	using path = std::filesystem::path;
	// Sanity check so that everything works by default
	REQUIRE(Parser::parseString(code).has_value());

	auto config = Parser::Config();
	config.m_systemIncludes = {"-isystem" +
	                           std::string(path("imaginary") / path("path"))};
	CAPTURE(config.m_systemIncludes[0]);
	// Incorrect system header paths should yield no parsed namespace
	REQUIRE(!Parser::parseString(code, config).has_value());
}
