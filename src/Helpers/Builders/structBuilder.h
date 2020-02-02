#pragma once

#include "IR/ir.hpp"
#include "IRProxy/IRData.h"
#include <vector>

namespace Helpers::Builders {
/**
  * Convert the Helpers::Struct to IR::Struct and add them to the structure
  */
void buildStructs(std::vector<IRProxy::Struct>& structs,
                  IR::Namespace& globalNamespace);
}    // namespace Helpers::Builders
