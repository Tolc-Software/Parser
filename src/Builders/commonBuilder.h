#pragma once

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
* @return: std::deque<std::pair<std::string, IRProxy::Type>>
*/
std::deque<std::pair<std::string, IRProxy::Type>>
buildParentStructure(clang::DeclContext const* parent,
                     std::deque<std::string> names);

/**
* @brief: Tries to create an IR::Type (Int, Void, String, ...) of the input
*         If it fails it returns std::nullopt
*         NOTE: Assumes the qualifiers are removed (const, ...)
*
* @param: std::string_view type
*
* @return: std::optional<IR::Type>
*/
std::optional<IR::Type> getType(std::string_view type);

/**
* @brief: Tries to create an IR::Qualifier (Const, ...) of the input
*         If there are no qualifiers or if it fails it returns an empty vector
*         NOTE: Assumes the qualifiers are removed (const, ...)
*
* @param: clang::QualType type
*
* @return: std::vector<IR::Qualifier>
*/
std::vector<IR::Qualifier> getQualifiers(clang::QualType const& type);
}    // namespace Builders
