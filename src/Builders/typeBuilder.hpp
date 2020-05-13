#pragma once

#include "Helpers/Utils/split.hpp"
#include "IRProxy/IRData.hpp"
#include <clang/AST/Type.h>
#include <deque>
#include <optional>
#include <string>
#include <string_view>

namespace Builders {

/**
* @brief: Tries to create an IR::Type of the input
*         If it fails it returns std::nullopt
*
* @param: std::string_view type
*
* @return: std::optional<IR::Type>
*/
std::optional<IR::Type> buildType(clang::QualType type);

}    // namespace Builders