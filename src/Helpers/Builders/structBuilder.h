#pragma once
#include "Helpers/IRData.h"
#include "IR/ir.hpp"
#include <algorithm>
#include <deque>
#include <string>
#include <variant>
#include <vector>

namespace Helpers::Builders {
/**
  * Convert the Helpers::Struct to IR::Struct and add them to the structure
  */
void buildStructs(std::vector<Helpers::Struct>& structs,
                  IR::Namespace& globalNamespace);
}    // namespace Helpers::Builders
