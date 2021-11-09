#include "Builders/commonBuilder.hpp"
#include "Builders/functionBuilder.hpp"
#include "Builders/typeBuilder.hpp"
#include "Visitor/ParserVisitor.hpp"
#include <spdlog/spdlog.h>

namespace Visitor {
bool ParserVisitor::VisitFunctionDecl(clang::FunctionDecl* functionDecl) {
	if (isInSystemHeader(functionDecl)) {
		// Continue the AST search
		return true;
	}
	spdlog::debug(R"(Parsing function: "{}")",
	              functionDecl->getQualifiedNameAsString());

	IRProxy::Function parsedFunc;
	parsedFunc.m_fullyQualifiedName = functionDecl->getQualifiedNameAsString();
	parsedFunc.m_path =
	    Builders::buildStructure(functionDecl, IRProxy::Structure::Function);

	// This is passed so that while extracting text from types it is exactly what the user wrote
	auto policy =
	    clang::PrintingPolicy(functionDecl->getASTContext().getLangOpts());

	if (auto returnType =
	        Builders::buildType(functionDecl->getReturnType(), policy)) {
		parsedFunc.m_returnType = returnType.value();
	} else {
		spdlog::error(R"(Failed to parse return type "{}" for function "{}")",
		              functionDecl->getReturnType().getAsString(policy),
		              functionDecl->getQualifiedNameAsString());
		m_parsedSuccessfully = false;
		// Stop parsing
		return false;
	}

	for (auto& p : functionDecl->parameters()) {
		// TODO: Handle unsupported types
		if (auto argType = Builders::buildType(p->getType(), policy)) {
			IR::Variable arg;
			arg.m_name = p->getName();
			arg.m_type = argType.value();
			parsedFunc.m_arguments.push_back(arg);
		} else {
			spdlog::error(
			    R"(Failed to parse argument type "{}" for function "{}")",
			    p->getType().getAsString(policy),
			    functionDecl->getQualifiedNameAsString());
			m_parsedSuccessfully = false;
			// Stop parsing
			return false;
		}
	}

	// Check for access modifiers (public, private, ...)
	parsedFunc.m_modifier =
	    Builders::convertToIRAccess(functionDecl->getAccess());

	m_irData.m_functions.push_back(parsedFunc);

	// Continue the AST search
	return true;
}

}    // namespace Visitor
