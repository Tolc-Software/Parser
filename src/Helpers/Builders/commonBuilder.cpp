#include "Helpers/Builders/commonBuilder.h"
#include "Helpers/IRData.h"
#include "clang/AST/DeclBase.h"
#include <deque>
#include <string>

namespace Helpers::Builders {

Helpers::Type getType(clang::DeclContext const* decl) {
	if (decl->isRecord()) {
		return Helpers::Type::Struct;
	} else if (decl->isNamespace()) {
		return Helpers::Type::Namespace;
	}
	return Helpers::Type::Unknown;
}

std::deque<std::pair<std::string, Helpers::Type>>
buildParentStructure(clang::DeclContext const* parent,
                     std::deque<std::string> names) {
	std::deque<std::pair<std::string, Helpers::Type>> structure;
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

