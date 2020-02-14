#include "Builders/commonBuilder.h"
#include "Builders/functionBuilder.h"
#include "Builders/typeBuilder.h"
#include "Visitor/ParserVisitor.h"

namespace Visitor {

bool ParserVisitor::VisitFunctionDecl(clang::FunctionDecl* functionDecl) {
	if (isInSystemHeader(functionDecl)) {
		// Continue the AST search
		return true;
	}

	IRProxy::Function parsedFunc;
	parsedFunc.m_name =
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

	m_irData.m_functions.push_back(
	    {Builders::convertToIRAccess(functionDecl->getAccess()), parsedFunc});

	// Continue the AST search
	return true;
}

}
