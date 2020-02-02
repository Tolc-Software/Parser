#include "Helpers/Builders/commonBuilder.h"
#include "Helpers/IRData.h"
#include "Helpers/Utils/split.h"
#include "Visitor/ParserVisitor.h"
#include "clang/AST/Type.h"

#include <iostream>
#include <string>

namespace Visitor {

bool ParserVisitor::VisitCXXRecordDecl(clang::CXXRecordDecl* classDecl) {
	std::cout << "I'm called with: " << classDecl->getQualifiedNameAsString()
	          << '\n';
	auto splitNames =
	    Helpers::Utils::split(classDecl->getQualifiedNameAsString(), "::");

	// Only interested in the structure from here on up
	// We know this is a record
	auto nameOfRecord = splitNames.back();
	splitNames.pop_back();
	auto structure = Helpers::Builders::buildParentStructure(
	    classDecl->getParent(), splitNames);
	// Push the struct back in
	structure.push_back({nameOfRecord, Helpers::Type::Struct});

	// Build the final parsedStruct
	Helpers::Struct parsedStruct;
	parsedStruct.m_name = structure;

	m_irData.m_structs.push_back(parsedStruct);
	// Continue the AST search
	return true;
}

}
