#include "Helpers/Builders/commonBuilder.h"
#include "IRProxy/IRData.h"
#include "clang/AST/DeclBase.h"
#include <deque>
#include <string>

namespace Helpers::Builders {

IRProxy::Type getType(clang::DeclContext const* decl) {
	if (decl->isRecord()) {
		return IRProxy::Type::Struct;
	} else if (decl->isNamespace()) {
		return IRProxy::Type::Namespace;
	}
	return IRProxy::Type::Unknown;
}

std::deque<std::pair<std::string, IRProxy::Type>>
buildParentStructure(clang::DeclContext const* parent,
                     std::deque<std::string> names) {
	std::deque<std::pair<std::string, IRProxy::Type>> structure;
	while (parent && !names.empty()) {
		structure.push_back({names.back(), getType(parent)});
		parent = parent->getParent();
		// Make sure we don't crash
		if (names.empty()) {
			break;
		}
		names.pop_back();
	}
	return structure;
}

}    // namespace Helpers::Builders

