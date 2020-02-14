#pragma once

#include "Helpers/Utils/split.h"
#include "IRProxy/IRData.h"
#include <clang/AST/Type.h>
#include <deque>
#include <optional>
#include <string>
#include <string_view>

namespace Builders {

/**
* @brief: Create a deque combining the type and the names of each DeclContext
*         NOTE: Assumes the last in the names deque is the name of the first parent
*
* @param: clang::DeclContext const* parent,
*         std::deque<std::string> names
*
* @return: std::deque<std::pair<std::string, IRProxy::Structure>>
*/
std::deque<std::pair<std::string, IRProxy::Structure>>
buildParentStructure(clang::DeclContext const* parent,
                     std::deque<std::string> names);

/**
* @brief: Create a deque combining the type and the names of each DeclContext
*
* @param: ClangDecl const* clangDecl,
*         IRProxy::Structure baseType - The type of the ClangDecl
*
* @return: std::deque<std::pair<std::string, IRProxy::Structure>>
*/
template <typename ClangDecl>
std::deque<std::pair<std::string, IRProxy::Structure>>
buildStructure(ClangDecl const* clangDecl, IRProxy::Structure baseType) {
	auto splitNames =
	    Helpers::Utils::split(clangDecl->getQualifiedNameAsString(), "::");

	auto nameOfDecl = splitNames.back();
	splitNames.pop_back();
	// Build from where we don't know the declstructure
	auto structure =
	    Builders::buildParentStructure(clangDecl->getParent(), splitNames);
	// Push the base back in
	structure.push_back({nameOfDecl, baseType});
	return structure;
}

}    // namespace Builders
