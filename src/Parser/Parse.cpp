#include "Parser/Parse.h"
#include "Frontend/ParserFrontendAction.h"
#include "Helpers/namespaceBuilder.h"
#include "clang/Tooling/Tooling.h"
#include <string>
#include <vector>

namespace Parser {

std::vector<IR::Namespace> parseString(const std::string& code) {
	std::vector<IR::Namespace> parsedIR;
	clang::tooling::runToolOnCode(new Frontend::ParserFrontendAction(parsedIR),
	                              code);

	return parsedIR;
}

}    // namespace Parser
