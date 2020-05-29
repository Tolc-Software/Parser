#include "Builders/commonBuilder.hpp"
#include "IRProxy/IRData.hpp"
#include "Visitor/ParserVisitor.hpp"

namespace Visitor {

bool ParserVisitor::VisitEnumDecl(clang::EnumDecl* enumDecl) {
	if (isInSystemHeader(enumDecl)) {
		// Continue the AST search
		return true;
	}

	// Build the final parsedEnum
	IRProxy::Enum parsedEnum;

	parsedEnum.m_fullyQualifiedName = enumDecl->getQualifiedNameAsString();

	parsedEnum.m_path =
	    Builders::buildStructure(enumDecl, IRProxy::Structure::Enum);

	parsedEnum.m_isScoped = enumDecl->isScopedUsingClassTag();

	for (auto value : enumDecl->enumerators()) {
		parsedEnum.m_values.push_back(value->getNameAsString());
	}

	// Check for access modifiers (public, private, ...)
	parsedEnum.m_modifier = Builders::convertToIRAccess(enumDecl->getAccess());

	m_irData.m_enums.push_back(parsedEnum);
	// Continue the AST search
	return true;
}
}