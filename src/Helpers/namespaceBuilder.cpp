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

// namespace {
// void printNS(IR::Namespace const& ns) {
// 	std::cout << ns.m_name << '\n';
// 	std::cout << " - Parent: '" << ns.m_parent << "'" << '\n';
// 	std::cout << " - Children:" << '\n';
// 	for (auto& child : ns.m_children) {
// 		std::cout << "    '" << child.m_name << "'" << '\n';
// 	}
// 	std::cout << "---------------" << '\n';
// 	for (auto& child : ns.m_children) {
// 		printNS(child);
// 	}
// }
// }    // namespace

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

	// {name, namespace}
	std::unordered_map<std::string, IR::Namespace> lookup;

	// Build parent structure and lookup table
	for (auto& [parentName, ns] : namespaces) {
		lookup[ns.m_name] = ns;
		lookup[ns.m_name].m_parent = parentName;
	}

	// Build child structure
	for (auto& [name, ns] : lookup) {
		if (auto parent = lookup.find(ns.m_parent); parent != lookup.end()) {
			parent->second.m_children.push_back(ns);
		}
	}

	// Save the root nodes
	for (auto [nsName, ns] : lookup) {
		if (ns.m_parent.empty()) {
			builtNamespaces.push_back(ns);
		}
	}

	return builtNamespaces;
}
}    // namespace Helpers
