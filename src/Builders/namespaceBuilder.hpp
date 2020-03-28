#pragma once

#include <IR/ir.hpp>
#include <string>
#include <vector>

namespace Builders {
/**
  * Build the structure between namespaces (parent - children)
  * and return the root namespace (always the global namespace)
  */
IR::Namespace
buildNamespaceStructure(std::vector<std::string> const& namespaces);
}    // namespace Builders
