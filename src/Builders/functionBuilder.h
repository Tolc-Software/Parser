#pragma once

#include "IR/ir.hpp"
#include "IRProxy/IRData.h"
#include <clang/AST/Type.h>
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
  * Go through and remove the pointers of the type
  */
clang::QualType getTypeWithPointersRemoved(clang::QualType type);

/**
  * Count the number of pointers on QualType (int** -> 2, ...)
  */
int getNumberOfPointers(clang::QualType type);

/**
  * Convert the clang::AccessSpecifier to IR::AccessModifier and add them to the structure
  */
std::optional<IR::AccessModifier>
convertToIRAccess(clang::AccessSpecifier access);

}    // namespace Builders
