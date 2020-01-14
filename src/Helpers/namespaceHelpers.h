#pragma once

#include "IR/ir.hpp"
#include <string_view>
#include <vector>

namespace Helpers {

/**
  * Perform a linear search for the namespace with name as input
  */
std::vector<IR::Namespace>::iterator
findNamespace(std::vector<IR::Namespace>& namespaces, std::string_view name);
}
