#include "Helpers/IRData.h"
#include "Helpers/Utils/split.h"
#include "Visitor/ParserVisitor.h"
#include "clang/AST/Type.h"

#include <iostream>
#include <string>

namespace {

/**
* @brief: Determine the type of the decl
*
* @param: clang::DeclContext const* decl
*
* @return: 	Helpers::Type
*/
Helpers::Type getType(clang::DeclContext const* decl) {
	if (decl->isRecord()) {
		return Helpers::Type::Struct;
	} else if (decl->isNamespace()) {
		return Helpers::Type::Namespace;
	}
	return Helpers::Type::Unknown;
}

/**
* @brief: Create a deque combining the type and the names of each DeclContext
*         NOTE: Assumes the last in the names deque is the name of the first parent
*
* @param: clang::DeclContext const* parent,
*         std::deque<std::string> names
*
* @return: std::deque<std::pair<std::string, Helpers::Type>>
*/
std::deque<std::pair<std::string, Helpers::Type>>
buildParentStructure(clang::DeclContext const* parent,
                     std::deque<std::string> names) {
	std::deque<std::pair<std::string, Helpers::Type>> structure;
	while (parent && !names.empty()) {
		structure.push_back({names.back(), getType(parent)});
		parent = parent->getParent();
		// Make sure we don't crash
		if (names.empty()) {
			break;
		}
		names.pop_back();
	}
	return structure;
}
}    // namespace

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
	auto structure = buildParentStructure(classDecl->getParent(), splitNames);
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
