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

	std::cout << "I'm called with: " << classDecl->getQualifiedNameAsString()
	          << '\n';
	auto splitNames =
	    Helpers::Utils::split(classDecl->getQualifiedNameAsString(), "::");

	// Only interested in the structure from here on up
	// We know this is a record
	auto nameOfRecord = splitNames.back();
	splitNames.pop_back();
	auto structure =
	    Builders::buildParentStructure(classDecl->getParent(), splitNames);
	// Push the struct back in
	structure.push_back({nameOfRecord, IRProxy::Type::Struct});

	// Build the final parsedStruct
	IRProxy::Struct parsedStruct;
	parsedStruct.m_name = structure;

	m_irData.m_structs.push_back(parsedStruct);
	// Continue the AST search
	return true;
}

}
