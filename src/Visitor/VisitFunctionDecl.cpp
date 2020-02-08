#include "Builders/commonBuilder.h"
#include "Helpers/Utils/split.h"
#include "Visitor/ParserVisitor.h"
#include "clang/AST/Type.h"

#include <iostream>
#include <string>

namespace Visitor {

bool ParserVisitor::VisitFunctionDecl(clang::FunctionDecl* functionDecl) {
	if (isInSystemHeader(functionDecl)) {
		// Continue the AST search
		return true;
	}

	auto splitNames =
	    Helpers::Utils::split(functionDecl->getQualifiedNameAsString(), "::");

	// Only interested in the structure from here on up
	// We know this is a function
	auto nameOfFunction = splitNames.back();
	splitNames.pop_back();
	auto structure =
	    Builders::buildParentStructure(functionDecl->getParent(), splitNames);
	// Push the function back in
	structure.push_back({nameOfFunction, IRProxy::Type::Function});

	IRProxy::Function parsedFunc;
	parsedFunc.m_name = structure;

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
