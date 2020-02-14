#include "Builders/commonBuilder.h"
#include "IRProxy/IRData.h"
#include <IR/ir.hpp>
#include <clang/AST/DeclBase.h>
#include <deque>
#include <optional>
#include <string>
#include <string_view>

namespace Builders {

std::optional<IRProxy::Structure>
getProxyStructure(clang::DeclContext const* decl) {
	if (decl->isNamespace()) {
		return IRProxy::Structure::Namespace;
	} else if (decl->isRecord()) {
		return IRProxy::Structure::Struct;
	} else if (decl->isFunctionOrMethod()) {
		return IRProxy::Structure::Function;
	}
	return {};
}

std::deque<std::pair<std::string, IRProxy::Structure>>
buildParentStructure(clang::DeclContext const* parent,
                     std::deque<std::string> names) {
	std::deque<std::pair<std::string, IRProxy::Structure>> structure;
	while (parent && !names.empty()) {
		if (auto proxyStructure = getProxyStructure(parent)) {
			structure.push_back({names.back(), proxyStructure.value()});
		}
		// TODO: Handle not being able to find the correct structure
		parent = parent->getParent();
		names.pop_back();
	}
	return structure;
}

}    // namespace Builders

