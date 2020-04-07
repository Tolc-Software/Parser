#include "Builders/commonBuilder.hpp"
#include "Builders/functionBuilder.hpp"
#include "Builders/typeBuilder.hpp"
#include "Visitor/ParserVisitor.hpp"

namespace Visitor {

bool ParserVisitor::VisitFunctionDecl(clang::FunctionDecl* functionDecl) {
	if (isInSystemHeader(functionDecl)) {
		// Continue the AST search
		return true;
	}

	IRProxy::Function parsedFunc;
	parsedFunc.m_fullyQualifiedName = functionDecl->getQualifiedNameAsString();
	parsedFunc.m_path =
	    Builders::buildStructure(functionDecl, IRProxy::Structure::Function);

	if (auto returnType = Builders::buildType(functionDecl->getReturnType())) {
		parsedFunc.m_returnType = returnType.value();
	}

	for (auto& p : functionDecl->parameters()) {
		// TODO: Handle unsupported types
		if (auto argType = Builders::buildType(p->getType())) {
			IR::Variable arg;
			arg.m_name = p->getName();
			arg.m_type = argType.value();
			parsedFunc.m_arguments.push_back(arg);
		}
	}

	// Check for access modifiers (public, private, ...)
	parsedFunc.m_modifier = Builders::convertToIRAccess(functionDecl->getAccess());

	m_irData.m_functions.push_back(parsedFunc);

	// Continue the AST search
	return true;
}

}
