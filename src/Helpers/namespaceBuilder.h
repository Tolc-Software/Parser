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
  * Check if parent of input is a namespace, if so, return its name
  * Otherwise return ""
  */
std::vector<IR::Namespace> buildNamespaceStructure(
    std::vector<std::pair<std::string, IR::Namespace>> const& namespaces);
}    // namespace Helpers
