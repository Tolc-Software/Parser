#include "Frontend/ParserFrontendAction.h"
#include "clang/Tooling/Tooling.h"

int main(int /*argc*/, char** /*argv*/) {
	// if (argc > 1) {
	// 	clang::tooling::runToolOnCode(new ParserFrontendAction(), argv[1]);
	// }

	// clang::tooling::runToolOnCode(
	//     new ParserFrontendAction(),
	//     "namespace n { namespace m { class C {}; } }");

	clang::tooling::runToolOnCode(new Frontend::ParserFrontendAction(),
	                              R"(
int FunctionName(const double d, int i) {
	return d + 5;
}
		)");
}
