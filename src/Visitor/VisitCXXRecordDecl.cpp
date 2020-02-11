#include "Builders/commonBuilder.h"
#include "IRProxy/IRData.h"
#include "Visitor/ParserVisitor.h"

namespace Visitor {

bool ParserVisitor::VisitCXXRecordDecl(clang::CXXRecordDecl* classDecl) {
	if (isInSystemHeader(classDecl)) {
		// Continue the AST search
		return true;
	}

	// Build the final parsedStruct
	IRProxy::Struct parsedStruct;
	parsedStruct.m_name =
	    Builders::buildStructure(classDecl, IRProxy::Structure::Struct);

	m_irData.m_structs.push_back(parsedStruct);
	// Continue the AST search
	return true;
}

}
