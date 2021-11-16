#include "Builders/commonBuilder.hpp"
#include "IRProxy/IRData.hpp"
#include "Visitor/ParserVisitor.hpp"
#include <spdlog/spdlog.h>

namespace Visitor {

bool ParserVisitor::VisitCXXRecordDecl(clang::CXXRecordDecl* classDecl) {
	if (isInSystemHeader(classDecl)) {
		// Continue the AST search
		return true;
	}

	if (isPureTemplate(classDecl)) {
		// Save for later parsing
		m_templateCache.m_structs.push_back(classDecl);
		// Continue the AST search
		return true;
	}
	if (isTemplateSpecialization(classDecl)) {
		// Continue the AST search
		return true;
	}

	spdlog::debug(R"(Parsing class/struct: "{}")",
	              classDecl->getQualifiedNameAsString());

	// Build the final parsedStruct
	IRProxy::Struct parsedStruct;

	parsedStruct.m_fullyQualifiedName = classDecl->getQualifiedNameAsString();

	parsedStruct.m_path =
	    Builders::buildStructure(classDecl, IRProxy::Structure::Struct);

	parsedStruct.m_hasImplicitDefaultConstructor =
	    classDecl->needsImplicitDefaultConstructor();

	m_irData.m_structs.push_back(parsedStruct);
	// Continue the AST search
	return true;
}

}    // namespace Visitor
