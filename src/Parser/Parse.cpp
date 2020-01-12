#include "Parser/Parse.h"
#include "Frontend/ParserFrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include <string>
#include <vector>

namespace Parser {

std::vector<IR::Namespace> parseString(const std::string& code) {
	std::vector<IR::Namespace> parsedIR;
	clang::tooling::runToolOnCode(new Frontend::ParserFrontendAction(parsedIR),
	                              code.c_str());

	return parsedIR;
}

}    // namespace Parser
