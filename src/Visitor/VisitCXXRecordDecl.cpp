#include "Builders/commonBuilder.h"
#include "Helpers/Utils/split.h"
#include "IRProxy/IRData.h"
#include "Visitor/ParserVisitor.h"
#include "clang/AST/Type.h"

#include <iostream>
#include <string>

namespace Visitor {

bool ParserVisitor::VisitCXXRecordDecl(clang::CXXRecordDecl* classDecl) {
	if (isInSystemHeader(classDecl)) {
		// Continue the AST search
		return true;
	}

	// Build the final parsedStruct
	IRProxy::Struct parsedStruct;
	parsedStruct.m_name =
	    Builders::buildStructure(classDecl, IRProxy::Type::Struct);

	m_irData.m_structs.push_back(parsedStruct);
	// Continue the AST search
	return true;
}

}
