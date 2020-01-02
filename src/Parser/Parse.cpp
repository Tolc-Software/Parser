#include "Parser/Parse.h"
#include "Frontend/ParserFrontendAction.h"
#include "Helpers/IRData.h"
#include "IR/ir.hpp"
#include "clang/Tooling/Tooling.h"
#include <string>
#include <vector>

namespace Parser {

std::vector<IR::Namespace> parseString(const std::string& code) {
	std::vector<IR::Namespace> namespaces;

	Helpers::IRData irData;
	clang::tooling::runToolOnCode(new Frontend::ParserFrontendAction(irData),
	                              code);

	return namespaces;
}

}    // namespace Parser
