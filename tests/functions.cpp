#include "IR/ir.hpp"
#include "Parser/Parse.h"
#include "catch2/catch.hpp"

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
