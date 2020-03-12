#include "Parser/Parse.h"
#include "Frontend/ParserFrontendAction.h"
#include "Helpers/includes.h"
#include "clang/Tooling/Tooling.h"
#include <iostream>
#include <string>

namespace Parser {

IR::Namespace parseString(std::string const& code) {
	std::cout << "====================" << '\n';
	std::cout << "Parsing code:" << '\n';
	std::cout << code << '\n';
	IR::Namespace parsedIR;

	clang::tooling::runToolOnCodeWithArgs(
	    new Frontend::ParserFrontendAction(parsedIR),
	    code.c_str(),
	    Helpers::getSystemIncludes());

	return parsedIR;
}

}    // namespace Parser
