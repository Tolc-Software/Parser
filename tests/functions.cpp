#include "IR/ir.hpp"
#include "Parser/Parse.h"
#include "catch2/catch.hpp"

namespace {
std::vector<IR::Type> getTypes() {
	using IR::Type;
	return {
	    Type::Char,
	    Type::Double,
	    Type::Float,
	    Type::Int,
	    Type::Long,
	    // Type::String, Not testing yet
	    Type::Void,
	};
}

std::string getTypeAsString(IR::Type type) {
	std::string s = "";
	using IR::Type;
	switch (type) {
		case Type::Char: s = "char"; break;
		case Type::Double: s = "double"; break;
		case Type::Float: s = "float"; break;
		case Type::Int: s = "int"; break;
		case Type::Long: s = "long"; break;
		case Type::String: s = "std::string"; break;
		case Type::Void: s = "void"; break;
	}
	return s;
}

std::string getValidReturnForType(IR::Type type) {
	std::string validReturn = "";
	using IR::Type;
	switch (type) {
		case Type::Char: validReturn = "'c'"; break;
		case Type::Double: validReturn = "1.0"; break;
		case Type::Float: validReturn = "1.0"; break;
		case Type::Int: validReturn = "1"; break;
		case Type::Long: validReturn = "1"; break;
		case Type::String: validReturn = "\"HelloWorld\""; break;
		case Type::Void: validReturn = ""; break;
	}
	return validReturn;
}
}

TEST_CASE("Simple function", "[functions]") {
	auto globalNS = Parser::parseString(R"(
void fun() {}
		)");
	SECTION("Parser finds the function") {
		REQUIRE(globalNS.m_functions.size() == 1);
		auto fun = globalNS.m_functions[0];
		CHECK(fun.m_name == "fun");
		CHECK(fun.m_arguments.size() == 0);
		CHECK(fun.m_returnType == IR::Type::Void);
	}
}

TEST_CASE("Function with different returns", "[functions]") {
	for (auto type : getTypes()) {
		auto stringType = getTypeAsString(type);
		std::string code = stringType + " fun() { return " +
		                   getValidReturnForType(type) + "; }";
		auto globalNS = Parser::parseString(code);
		// Print on error
		CAPTURE(code);
		CAPTURE(stringType);

		SECTION("Parser finds the function") {
			REQUIRE(globalNS.m_functions.size() == 1);
			auto fun = globalNS.m_functions[0];
			CHECK(fun.m_name == "fun");
			CHECK(fun.m_arguments.size() == 0);
			SECTION("With correct return type") {
				CHECK(fun.m_returnType == type);
			}
		}
	}
}

// TEST_CASE("With return type", "[functions]") {
// 	auto globalNS = Parser::parseString(R"(
// int fun() {
// 	return 5;
// }
// 		)");
// }

// TEST_CASE("Return type and simple argument", "[functions]") {
// 	auto globalNS = Parser::parseString(R"(
// int fun(int i) {
// 	return i;
// }
// 		)");
// }

// TEST_CASE("Return type with const qualifier", "[functions]") {
// 	auto globalNS = Parser::parseString(R"(
// int fun(int const i) {
// 	return i;
// }
// 		)");
// }

// TEST_CASE("Multiple return types", "[functions]") {
// 	auto globalNS = Parser::parseString(R"(
// int fun(const int i, double d) {
// 	return d + 5;
// }
// 		)");
// }

// TEST_CASE("Pointer argument", "[functions]") {
// 	auto globalNS = Parser::parseString(R"(
// char fun(char* c) {
// 	return *c;
// }
// 		)");
// }

// TEST_CASE("Pointer argument with const", "[functions]") {
// 	auto globalNS = Parser::parseString(R"(
// char const fun(char const * c) {
// 	return *c;
// }
// 		)");
// }
