#include "Helpers/namespaceBuilder.h"
#include "Helpers/namespaceHelpers.h"
#include "IR/ir.hpp"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclBase.h"
#include "llvm/Support/Casting.h"
#include <algorithm>
#include <iostream>
#include <string_view>
#include <vector>

namespace {

IR::Namespace const* getIRNamespace(std::vector<IR::Namespace>& namespaces,
                                    clang::NamespaceDecl const* ns) {
	if (ns) {
		auto foundNs =
		    Helpers::findNamespace(namespaces, std::string(ns->getName()));
		if (foundNs != namespaces.end()) {
			return &(*foundNs);
		}
	}
	return nullptr;
}
}    // namespace

namespace Helpers {

std::vector<IR::Namespace> buildNamespaceStructure(
    std::vector<std::pair<clang::DeclContext const*, IR::Namespace>> const&
        namespaces) {
	std::vector<IR::Namespace> builtNamespaces;

	for (auto& [parentDecl, ns] : namespaces) {
		builtNamespaces.push_back(ns);
	}

	for (auto& [parentDecl, ns] : namespaces) {
		if (parentDecl->isNamespace() && !parentDecl->isStdNamespace()) {
			if (auto parent = getIRNamespace(
			        builtNamespaces,
			        static_cast<clang::NamespaceDecl const*>(parentDecl))) {
				if (auto builtNs =
				        Helpers::findNamespace(builtNamespaces, ns.m_name);
				    builtNs != builtNamespaces.end()) {
					builtNs->m_parent = parent->m_name;
				}
			}
		}
	}

	return builtNamespaces;
}
}    // namespace Helpers
