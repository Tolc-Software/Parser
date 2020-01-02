#include "Parser/Parse.h"
#include "Frontend/ParserFrontendAction.h"
#include "IR/ir.hpp"
#include "clang/Tooling/Tooling.h"
#include <string>
#include <vector>

namespace Parser {

std::vector<IR::Namespace> parseString(const std::string& code) {
	std::vector<IR::Namespace> namespaces;

	clang::tooling::runToolOnCode(new Frontend::ParserFrontendAction(), code);

	return namespaces;
}

}    // namespace Parser
