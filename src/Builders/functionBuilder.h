#pragma once

#include "IR/ir.hpp"
#include "IRProxy/IRData.h"
#include <clang/Basic/Specifiers.h>
#include <vector>

namespace Builders {
/**
  * Convert the IRProxy::Function to IR::Function and add them to the structure
  */
void buildFunctions(
    const std::vector<std::pair<std::optional<IR::AccessModifier>,
                                IRProxy::Function>>& functions,
    IR::Namespace& globalNamespace);

/**
  * Convert the clang::AccessSpecifier to IR::AccessModifier and add them to the structure
  */
std::optional<IR::AccessModifier>
convertToIRAccess(clang::AccessSpecifier access);

}    // namespace Builders
