#include "Builders/commonBuilder.hpp"
#include "Builders/typeBuilder.hpp"
#include "Helpers/Type/utilities.hpp"
#include "IRProxy/IRData.hpp"
#include "Visitor/ParserVisitor.hpp"
#include <spdlog/spdlog.h>

namespace Visitor {
bool ParserVisitor::VisitFieldDecl(clang::FieldDecl* fieldDecl) {
	if (isInSystemHeader(fieldDecl)) {
		// Continue the AST search
		return true;
	}
	spdlog::debug(R"(Parsing member variable: "{}")",
	              fieldDecl->getQualifiedNameAsString());

	IRProxy::MemberVariable proxyVariable;

	IR::Variable variable;
	variable.m_name = fieldDecl->getName();

	// This is passed so that while extracting text from types it is exactly what the user wrote
	auto policy =
	    clang::PrintingPolicy(fieldDecl->getASTContext().getLangOpts());

	if (auto type = Builders::buildType(fieldDecl->getType(), policy)) {
		variable.m_type = type.value();
	} else {
		spdlog::error(R"(Failed to parse type "{}" for member variable "{}")",
		              fieldDecl->getType().getAsString(policy),
		              fieldDecl->getQualifiedNameAsString());
		m_parsedSuccessfully = false;
		// Stop parsing
		return false;
	}

	proxyVariable.m_variable = variable;

	// TODO: Handle access modifier not being converted
	if (auto accessModifier =
	        Builders::convertToIRAccess(fieldDecl->getAccess())) {
		proxyVariable.m_modifier = accessModifier.value();
	} else {
		spdlog::error(
		    R"(Failed to parse access modifier for member variable "{}")",
		    fieldDecl->getQualifiedNameAsString());
		m_parsedSuccessfully = false;
		// Stop parsing
		return false;
	}

	auto parentName = fieldDecl->getParent()->getQualifiedNameAsString();

	m_irData.m_memberVariables[parentName].push_back(proxyVariable);

	// Continue the AST search
	return true;
}
}    // namespace Visitor
