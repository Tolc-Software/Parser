#include "Frontend/ParserFrontendAction.h"
#include "catch2/catch.hpp"
#include "clang/Tooling/Tooling.h"

TEST_CASE("Simple function", "[functions]") {
	clang::tooling::runToolOnCode(new Frontend::ParserFrontendAction(),
	                              R"(
void FunctionName() {}
		)");
}

TEST_CASE("With return type", "[functions]") {
	clang::tooling::runToolOnCode(new Frontend::ParserFrontendAction(),
	                              R"(
int FunctionName() {
	return 5;
}
		)");
}

TEST_CASE("Return type and simple argument", "[functions]") {
	clang::tooling::runToolOnCode(new Frontend::ParserFrontendAction(),
	                              R"(
int FunctionName(int i) {
	return i;
}
		)");
}

TEST_CASE("Return type with const qualifier", "[functions]") {
	clang::tooling::runToolOnCode(new Frontend::ParserFrontendAction(),
	                              R"(
int FunctionName(int const i) {
	return i;
}
		)");
}

TEST_CASE("Multiple return types", "[functions]") {
	clang::tooling::runToolOnCode(new Frontend::ParserFrontendAction(),
	                              R"(
int FunctionName(const int i, double d) {
	return d + 5;
}
		)");
}

TEST_CASE("Pointer argument", "[functions]") {
	clang::tooling::runToolOnCode(new Frontend::ParserFrontendAction(),
	                              R"(
char FunctionName(char* c) {
	return *c;
}
		)");
}

TEST_CASE("Pointer argument with const", "[functions]") {
	clang::tooling::runToolOnCode(new Frontend::ParserFrontendAction(),
	                              R"(
char const FunctionName(char const * c) {
	return *c;
}
		)");
}
