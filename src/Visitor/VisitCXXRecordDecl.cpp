#include "Builders/commonBuilder.hpp"
#include "IRProxy/IRData.hpp"
#include "Visitor/ParserVisitor.hpp"

namespace Visitor {

bool ParserVisitor::VisitCXXRecordDecl(clang::CXXRecordDecl* classDecl) {
	if (isInSystemHeader(classDecl)) {
		// Continue the AST search
		return true;
	}

	// Build the final parsedStruct
	IRProxy::Struct parsedStruct;

	parsedStruct.m_fullyQualifiedName = classDecl->getQualifiedNameAsString();

	parsedStruct.m_path =
	    Builders::buildStructure(classDecl, IRProxy::Structure::Struct);

	m_irData.m_structs.push_back(parsedStruct);
	// Continue the AST search
	return true;
}

}
