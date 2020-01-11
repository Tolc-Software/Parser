#include "IR/ir.hpp"
#include "clang/AST/DeclBase.h"
#include "llvm/Support/Casting.h"
#include <vector>

namespace Helpers {

std::vector<IR::Namespace> buildNamespaceStructure(
    std::vector<std::pair<clang::DeclContext const*, IR::Namespace>> const&
        namespaces) {
	std::vector<IR::Namespace> builtNamespaces;
	for (auto [parent, ns] : namespaces) {
		if (parent->isNamespace()) {
		}
	}
	return {};
}
}    // namespace Helpers
