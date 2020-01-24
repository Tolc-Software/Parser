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

void printNSInternal(IR::Namespace const& ns, int depth) {
	std::string indentation;
	for (int i = 0; i < depth; ++i) {
		indentation += "    ";
	}
	std::cout << indentation << ns.m_name << '\n';
	std::cout << indentation << " - Parent: '" << ns.m_parent << "'" << '\n';
	std::cout << indentation << " - Children:" << '\n';
	for (auto& child : ns.m_children) {
		std::cout << indentation << "    '" << child.m_name << "'" << '\n';
	}
	for (auto& child : ns.m_children) {
		std::cout << indentation << "    "
		          << "|" << '\n';
		printNSInternal(child, depth + 1);
	}
	std::cout << "---------------" << '\n';
}
void printNS(IR::Namespace const& ns) {
	// Start the recursion
	printNSInternal(ns, 0);
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

IR::Namespace buildNamespaceStructure(
    std::vector<std::pair<std::string, IR::Namespace>> const& namespaces) {
	// Output
	std::vector<IR::Namespace> builtNamespaces;

	// {name, namespace}
	std::unordered_map<std::string, IR::Namespace> lookup;

	std::cout << "================================" << '\n' << '\n';
	std::cout << "Starting" << '\n';
	// Build parent structure and lookup table
	for (auto& [parentName, ns] : namespaces) {
		lookup[ns.m_name] = ns;
		lookup[ns.m_name].m_parent = parentName;
	}
	lookup[""] = IR::Namespace();

	// Build child structure
	for (auto& [name, ns] : lookup) {
		// The global namespace is not a child :)
		if (!name.empty()) {
			std::cout << "On namespace: " << name << '\n';
			if (auto parent = lookup.find(ns.m_parent);
			    parent != lookup.end()) {
				std::cout << "Adding [parent, child]: [" << parent->first
				          << ", " << name << "]" << '\n';
				parent->second.m_children.push_back(ns);
			}
		}
	}

	std::cout << "Ending at:" << '\n';
	for (auto& [name, ns] : lookup) {
		printNS(ns);
	}

	// Return the root node
	return lookup[""];
}
}    // namespace Helpers
