#include "Builders/commonBuilder.h"
#include "IRProxy/IRData.h"
#include <IR/ir.hpp>
#include <clang/AST/DeclBase.h>
#include <deque>
#include <optional>
#include <string>
#include <string_view>

namespace {
std::optional<IR::BaseType> getBaseType(std::string_view type) {
	using IR::BaseType;
	if (type == "char") {
		return BaseType::Char;
	} else if (type == "double") {
		return BaseType::Double;
	} else if (type == "float") {
		return BaseType::Float;
	} else if (type == "int") {
		return BaseType::Int;
	} else if (type == "long") {
		return BaseType::Long;
	} else if (type == "string") {
		return BaseType::String;
	} else if (type == "void") {
		return BaseType::Void;
	}
	return {};
}

IR::Type buildTypeFromBase(IR::BaseType valueType,
                           std::optional<IR::BaseType> keyType = std::nullopt) {
	IR::Type::Value v;
	v.m_valueType = valueType;
	v.m_keyType = keyType;

	IR::Type type;
	type.m_type = v;
	return type;
}

}    // namespace

namespace Builders {

std::optional<IR::Type> getType(std::string_view type) {
	if (auto baseType = getBaseType(type)) {
		return buildTypeFromBase(baseType.value());
	}
	// TODO: Support containers
	// else if (auto containerType = getContainerType(type)) {
	// return buildTypeFromContainer(containerType.value());
	// }
	return {};
}

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

std::vector<IR::Qualifier> getQualifiers(clang::QualType const& type) {
	std::vector<IR::Qualifier> quals;
	auto qs = type.split().Quals;
	if (qs.hasConst()) {
		quals.push_back(IR::Qualifier::Const);
	}
	return quals;
}
}    // namespace Builders
