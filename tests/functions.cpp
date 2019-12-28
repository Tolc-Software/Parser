#include "Frontend/ParserFrontendAction.h"
#include "catch2/catch.hpp"
#include "clang/Tooling/Tooling.h"

TEST_CASE("Run tool", "[functions]") {
	clang::tooling::runToolOnCode(new Frontend::ParserFrontendAction(),
	                              R"(
int FunctionName(const double d, int i) {
	return d + 5;
}
		)");
}
