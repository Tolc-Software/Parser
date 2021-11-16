#include "Builders/commonBuilder.hpp"
#include "Builders/functionBuilder.hpp"
#include "Builders/typeBuilder.hpp"
#include "Visitor/ParserVisitor.hpp"
#include <spdlog/spdlog.h>

namespace Visitor {
bool ParserVisitor::VisitFunctionDecl(clang::FunctionDecl* functionDecl) {
	if (isInSystemHeader(functionDecl) || isPureTemplate(functionDecl)) {
		// Continue the AST search
		return true;
	}
	spdlog::debug(R"(Parsing function: "{}")",
	              functionDecl->getQualifiedNameAsString());
	auto [status, parsedFunc] = Builders::buildFunction(functionDecl);
	switch (status) {
		case (Builders::FunctionError::Ok):

			m_irData.m_functions.push_back(parsedFunc.value());
			break;
		case (Builders::FunctionError::ArgumentType):
			spdlog::error(R"(Failed to parse argument type for function "{}")",
			              functionDecl->getQualifiedNameAsString());
			m_parsedSuccessfully = false;
			// Stop parsing
			return false;
			break;
		case (Builders::FunctionError::ReturnType):
			spdlog::error(R"(Failed to parse return type for function "{}")",
			              functionDecl->getQualifiedNameAsString());
			m_parsedSuccessfully = false;
			// Stop parsing
			return false;
			break;
	}

	// Continue the AST search
	return true;
}

}    // namespace Visitor
