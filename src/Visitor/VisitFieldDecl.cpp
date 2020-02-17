#include "Builders/commonBuilder.h"
#include "IRProxy/IRData.h"
#include "Visitor/ParserVisitor.h"
#include "Builders/typeBuilder.h"

namespace Visitor {

bool ParserVisitor::VisitFieldDecl(clang::FieldDecl* fieldDecl) {
	if (isInSystemHeader(fieldDecl)) {
		// Continue the AST search
		return true;
	}

	IRProxy::MemberVariable proxyVariable;
	IR::Variable variable;
	auto splitNames =
	    Helpers::Utils::split(fieldDecl->getQualifiedNameAsString(), "::");

	variable.m_name = splitNames.back();
	// TODO: Handle type not being converted
	if (auto type = Builders::buildType(fieldDecl->getType())) {
		variable.m_type = type.value();
	}

	proxyVariable.m_variable = variable;

	// TODO: Handle access modifier not being converted
	if (auto accessModifier = Builders::convertToIRAccess(fieldDecl->getAccess())) {
		proxyVariable.m_modifier = accessModifier.value();
	}

	auto parentName = fieldDecl->getParent()->getQualifiedNameAsString();

	m_irData.m_memberVariables[parentName].push_back(proxyVariable);

	// Continue the AST search
	return true;
}

}
