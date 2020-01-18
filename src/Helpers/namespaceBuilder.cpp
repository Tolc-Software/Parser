#include "Helpers/namespaceBuilder.h"
#include "Helpers/namespaceHelpers.h"
#include "IR/ir.hpp"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclBase.h"
#include "llvm/Support/Casting.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

IR::Namespace const* getIRNamespace(std::vector<IR::Namespace>& namespaces,
                                    std::string_view ns) {
	if (!ns.empty()) {
		auto foundNs = Helpers::findNamespace(namespaces, ns);
		if (foundNs != namespaces.end()) {
			return &(*foundNs);
		}
	}
	return nullptr;
}
}    // namespace

namespace Helpers {

std::string getParentNamespaceName(clang::NamespaceDecl const* namespaceDecl) {
	auto parentDecl = namespaceDecl->getParent();
	if (parentDecl->isNamespace() && !parentDecl->isStdNamespace()) {
		auto parentNS = static_cast<clang::NamespaceDecl const*>(parentDecl);
		return parentNS->getName();
	}
	return "";
}

std::vector<IR::Namespace> buildNamespaceStructure(
    std::vector<std::pair<std::string, IR::Namespace>> const& namespaces) {
	// Output
	std::vector<IR::Namespace> builtNamespaces;

	for (auto& [parent, ns] : namespaces) {
		builtNamespaces.push_back(ns);
	}

	for (auto& [parentName, ns] : namespaces) {
		if (auto parentNs = getIRNamespace(builtNamespaces, parentName)) {
			if (auto builtNs =
			        Helpers::findNamespace(builtNamespaces, ns.m_name);
			    builtNs != builtNamespaces.end()) {
				builtNs->m_parent = parentNs->m_name;
			}
		}
	}

	return builtNamespaces;
}
}    // namespace Helpers
