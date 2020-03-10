#include "Parser/Parse.h"
#include "Frontend/ParserFrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include <iostream>
#include <string>

namespace {
std::vector<std::string> getIncludes() {
	// return {"-I/usr/lib/clang/9.0.1/include/"};
	return {
	    "-I/home/simon/external/code/cpp/Parser/build/_deps/llvm_entry-src/lib/clang/9.0.0/include"};
}
}    // namespace

namespace Parser {

IR::Namespace parseString(std::string const& code) {
	std::cout << "====================" << '\n';
	std::cout << "Parsing code:" << '\n';
	std::cout << code << '\n';
	IR::Namespace parsedIR;
	// clang::tooling::runToolOnCode(new Frontend::ParserFrontendAction(parsedIR),
	//                               code.c_str());

	clang::tooling::runToolOnCodeWithArgs(
	    new Frontend::ParserFrontendAction(parsedIR),
	    code.c_str(),
	    getIncludes());

	return parsedIR;
}

}    // namespace Parser
