#include "Builders/commonBuilder.hpp"
#include "Builders/typeBuilder.hpp"
#include "Visitor/Helpers/parseFunction.hpp"
#include "Visitor/ParserVisitor.hpp"
#include <clang/AST/PrettyPrinter.h>
#include <spdlog/spdlog.h>

namespace Visitor {
bool ParserVisitor::VisitFunctionDecl(clang::FunctionDecl* functionDecl) {
	if (isInSystemHeader(functionDecl) || isPureTemplate(functionDecl)) {
		// Continue the AST search
		return true;
	}

	spdlog::debug(R"(Parsing function: "{}")",
	              functionDecl->getQualifiedNameAsString());

	if (auto parsedFunc = Visitor::Helpers::parseFunction(functionDecl)) {
		m_irData.m_functions.push_back(parsedFunc.value());
	} else {
		m_parsedSuccessfully = false;
		// Stop parsing
		return false;
	}

	// Continue the AST search
	return true;
}

}    // namespace Visitor
