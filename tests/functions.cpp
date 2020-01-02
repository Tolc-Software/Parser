#include "Frontend/ParserFrontendAction.h"
#include "Helpers/IRData.h"
#include "Parser/Parse.h"
#include "catch2/catch.hpp"
#include "clang/Tooling/Tooling.h"

TEST_CASE("Simple function", "[functions]") {
	auto namespaces = Parser::parseString(R"(
void FunctionName() {}
		)");
}

TEST_CASE("With return type", "[functions]") {
	auto namespaces = Parser::parseString(R"(
int FunctionName() {
	return 5;
}
		)");
}

TEST_CASE("Return type and simple argument", "[functions]") {
	auto namespaces = Parser::parseString(R"(
int FunctionName(int i) {
	return i;
}
		)");
}

TEST_CASE("Return type with const qualifier", "[functions]") {
	auto namespaces = Parser::parseString(R"(
int FunctionName(int const i) {
	return i;
}
		)");
}

TEST_CASE("Multiple return types", "[functions]") {
	auto namespaces = Parser::parseString(R"(
int FunctionName(const int i, double d) {
	return d + 5;
}
		)");
}

TEST_CASE("Pointer argument", "[functions]") {
	auto namespaces = Parser::parseString(R"(
char FunctionName(char* c) {
	return *c;
}
		)");
}

TEST_CASE("Pointer argument with const", "[functions]") {
	auto namespaces = Parser::parseString(R"(
char const FunctionName(char const * c) {
	return *c;
}
		)");
}
