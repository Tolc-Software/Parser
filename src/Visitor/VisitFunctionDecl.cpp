#include "Builders/commonBuilder.h"
#include "Visitor/ParserVisitor.h"

namespace Visitor {

bool ParserVisitor::VisitFunctionDecl(clang::FunctionDecl* functionDecl) {
	if (isInSystemHeader(functionDecl)) {
		// Continue the AST search
		return true;
	}

	IRProxy::Function parsedFunc;
	parsedFunc.m_name =
	    Builders::buildStructure(functionDecl, IRProxy::Type::Function);

	if (auto returnType = Builders::getType(
	        functionDecl->getReturnType().getUnqualifiedType().getAsString())) {
		parsedFunc.m_returnType = returnType.value();
	}

	for (auto& p : functionDecl->parameters()) {
		IR::Variable arg;
		arg.m_name = p->getName();
		if (auto argType = Builders::getType(
		        p->getType().getUnqualifiedType().getAsString())) {
			arg.m_type = argType.value();
		}
		// TODO: Handle unsupported qualifiers
		//       (has qualifiers but this function returns none)
		arg.m_qualifiers = Builders::getQualifiers(p->getType());

		parsedFunc.m_arguments.push_back(arg);
	}

	m_irData.m_functions.push_back(parsedFunc);

	// Continue the AST search
	return true;
}

}
