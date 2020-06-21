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
	spdlog::debug("Parsing function: {}",
	              functionDecl->getQualifiedNameAsString());

	IRProxy::Function parsedFunc;
	parsedFunc.m_fullyQualifiedName = functionDecl->getQualifiedNameAsString();
	parsedFunc.m_path =
	    Builders::buildStructure(functionDecl, IRProxy::Structure::Function);

	if (auto returnType = Builders::buildType(functionDecl->getReturnType())) {
		parsedFunc.m_returnType = returnType.value();
	} else {
		spdlog::error("Failed to parse return type {} for function {}",
		              functionDecl->getReturnType().getAsString(),
		              functionDecl->getQualifiedNameAsString());
		m_parsedSuccessfully = false;
		// Stop parsing
		return false;
	}

	for (auto& p : functionDecl->parameters()) {
		// TODO: Handle unsupported types
		if (auto argType = Builders::buildType(p->getType())) {
			IR::Variable arg;
			arg.m_name = p->getName();
			arg.m_type = argType.value();
			parsedFunc.m_arguments.push_back(arg);
		} else {
			spdlog::error("Failed to parse argument type {} for function {}",
			              functionDecl->getReturnType().getAsString(),
			              p->getType().getAsString());
			m_parsedSuccessfully = false;
			// Stop parsing
			return false;
		}
	}

	// Check for access modifiers (public, private, ...)
	parsedFunc.m_modifier = Builders::convertToIRAccess(functionDecl->getAccess());

	m_irData.m_functions.push_back(parsedFunc);

	// Continue the AST search
	return true;
}

}
