#include "Builders/commonBuilder.h"
#include "Helpers/Utils/split.h"
#include "IRProxy/IRData.h"
#include <IR/ir.hpp>
#include <clang/AST/DeclBase.h>
#include <deque>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

namespace Builders {

std::optional<IR::Type> getType(std::string_view type) {
	if (type == "char") {
		return IR::Type::Char;
	} else if (type == "double") {
		return IR::Type::Double;
	} else if (type == "float") {
		return IR::Type::Float;
	} else if (type == "int") {
		return IR::Type::Int;
	} else if (type == "long") {
		return IR::Type::Long;
	} else if (type == "string") {
		return IR::Type::String;
	} else if (type == "void") {
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

std::vector<IR::Qualifier> getQualifiers(clang::QualType const& type) {
	std::vector<IR::Qualifier> quals;
	auto qs = type.split().Quals;
	if (qs.hasConst()) {
		quals.push_back(IR::Qualifier::Const);
	}
	return quals;
}
}    // namespace Builders

