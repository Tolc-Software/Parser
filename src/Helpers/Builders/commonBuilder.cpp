#include "Helpers/Builders/commonBuilder.h"
#include "IRProxy/IRData.h"
#include <IR/ir.hpp>
#include <clang/AST/DeclBase.h>
#include <deque>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

namespace Helpers::Builders {

std::optional<IR::Type> getType(std::string_view type) {
	std::cout << "getType called with type: " << type << '\n';
	std::cout << (type == "void") << '\n';
	if (type == "void") {
		return IR::Type::Void;
	}
	return {};
}

IRProxy::Type getProxyDecl(clang::DeclContext const* decl) {
	if (decl->isNamespace()) {
		return IRProxy::Type::Namespace;
	} else if (decl->isRecord()) {
		return IRProxy::Type::Struct;
	} else if (decl->isFunctionOrMethod()) {
		return IRProxy::Type::Function;
	}
	return IRProxy::Type::Unknown;
}

std::deque<std::pair<std::string, IRProxy::Type>>
buildParentStructure(clang::DeclContext const* parent,
                     std::deque<std::string> names) {
	std::deque<std::pair<std::string, IRProxy::Type>> structure;
	while (parent && !names.empty()) {
		structure.push_back({names.back(), getProxyDecl(parent)});
		parent = parent->getParent();
		names.pop_back();
	}
	return structure;
}

}    // namespace Helpers::Builders

