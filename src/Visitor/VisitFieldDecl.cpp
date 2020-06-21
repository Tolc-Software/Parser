#include "Builders/commonBuilder.hpp"
#include "Builders/typeBuilder.hpp"
#include "IRProxy/IRData.hpp"
#include "Visitor/ParserVisitor.hpp"
#include <spdlog/spdlog.h>

namespace Visitor {
bool ParserVisitor::VisitFieldDecl(clang::FieldDecl* fieldDecl) {
	if (isInSystemHeader(fieldDecl)) {
		// Continue the AST search
		return true;
	}
	spdlog::debug("Parsing member variable: {}",
	              fieldDecl->getQualifiedNameAsString());

	IRProxy::MemberVariable proxyVariable;

	IR::Variable variable;
	variable.m_name = fieldDecl->getName();
	// TODO: Handle type not being converted
	if (auto type = Builders::buildType(fieldDecl->getType())) {
		variable.m_type = type.value();
	} else {
		spdlog::error("Failed to parse type {} for member variable {}",
		              fieldDecl->getType().getAsString(),
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
		spdlog::error("Failed to parse access modifier for member variable {}",
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
