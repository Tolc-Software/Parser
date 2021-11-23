#pragma once

#include <IR/ir.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace Builders {
/**
  * Build the structure between namespaces (parent - children)
  * and return the root namespace (always the global namespace)
  */
IR::Namespace
buildNamespaceStructure(std::vector<std::string> const& namespaces);

/**
  * Add the variables to the corresponding namespaces
  */
void addGlobalVariables(
    std::unordered_map<std::string, std::vector<IR::Variable>> const& variables,
    IR::Namespace& globalNS);
}    // namespace Builders
