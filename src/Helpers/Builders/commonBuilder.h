#pragma once

#include "IRProxy/IRData.h"
#include "clang/AST/Type.h"
#include <deque>
#include <string>

namespace Helpers::Builders {

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

}    // namespace Helpers::Builders
