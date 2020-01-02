#include "Frontend/ParserFrontendAction.h"
#include "Helpers/IRData.h"
#include "catch2/catch.hpp"
#include "clang/Tooling/Tooling.h"

TEST_CASE("Simple function", "[functions]") {
	Helpers::IRData d;
	clang::tooling::runToolOnCode(new Frontend::ParserFrontendAction(d),
	                              R"(
void FunctionName() {}
		)");
}

TEST_CASE("With return type", "[functions]") {
	Helpers::IRData d;
	clang::tooling::runToolOnCode(new Frontend::ParserFrontendAction(d),
	                              R"(
int FunctionName() {
	return 5;
}
		)");
}

TEST_CASE("Return type and simple argument", "[functions]") {
	Helpers::IRData d;
	clang::tooling::runToolOnCode(new Frontend::ParserFrontendAction(d),
	                              R"(
int FunctionName(int i) {
	return i;
}
		)");
}

TEST_CASE("Return type with const qualifier", "[functions]") {
	Helpers::IRData d;
	clang::tooling::runToolOnCode(new Frontend::ParserFrontendAction(d),
	                              R"(
int FunctionName(int const i) {
	return i;
}
		)");
}

TEST_CASE("Multiple return types", "[functions]") {
	Helpers::IRData d;
	clang::tooling::runToolOnCode(new Frontend::ParserFrontendAction(d),
	                              R"(
int FunctionName(const int i, double d) {
	return d + 5;
}
		)");
}

TEST_CASE("Pointer argument", "[functions]") {
	Helpers::IRData d;
	clang::tooling::runToolOnCode(new Frontend::ParserFrontendAction(d),
	                              R"(
char FunctionName(char* c) {
	return *c;
}
		)");
}

TEST_CASE("Pointer argument with const", "[functions]") {
	Helpers::IRData d;
	clang::tooling::runToolOnCode(new Frontend::ParserFrontendAction(d),
	                              R"(
char const FunctionName(char const * c) {
	return *c;
}
		)");
}
