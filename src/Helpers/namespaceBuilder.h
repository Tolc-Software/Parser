#pragma once

#include "clang/AST/DeclBase.h"
#include <IR/ir.hpp>
#include <vector>

namespace Helpers {
std::vector<IR::Namespace> buildNamespaceStructure(
    std::vector<std::pair<clang::DeclContext const*, IR::Namespace>> const&
        namespaces);
}    // namespace Helpers
