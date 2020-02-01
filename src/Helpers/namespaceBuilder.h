#pragma once

#include "clang/AST/DeclBase.h"
#include <IR/ir.hpp>
#include <vector>

namespace Helpers {
/**
  * Check if parent of input is a namespace, if so, return its name
  * Otherwise return ""
  */
std::string getParentNamespaceName(clang::NamespaceDecl const* namespaceDecl);

/**
  * Build the structure between namespaces (parent - children)
  * and return the root namespace (always the global namespace)
  */
IR::Namespace
buildNamespaceStructure(std::vector<std::string> const& namespaces);
}    // namespace Helpers
