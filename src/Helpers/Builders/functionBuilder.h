#pragma once

#include "IR/ir.hpp"
#include "IRProxy/IRData.h"
#include <vector>

namespace Helpers::Builders {
/**
  * Convert the Helpers::Function to IR::Function and add them to the structure
  */
void buildFunctions(std::vector<IRProxy::Function>& structs,
                    IR::Namespace& globalNamespace);
}    // namespace Helpers::Builders
