#pragma once

#include "IR/ir.hpp"
#include "IRProxy/IRData.hpp"
#include <vector>

namespace Builders {
/**
  * Convert the IRProxy::Function to IR::Function and add them to the structure
  */
void buildFunctions(
    const std::vector<IRProxy::Function>& functions,
    IR::Namespace& globalNamespace);

}    // namespace Builders
