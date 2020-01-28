#include "Helpers/namespaceBuilder.h"
#include "Helpers/namespaceHelpers.h"
#include "IR/ir.hpp"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclBase.h"
#include "llvm/Support/Casting.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
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
}
void printNS(IR::Namespace const& ns) {
	// Start the recursion
	printNSInternal(ns, 0);
	std::cout << "---------------" << '\n';
}

std::vector<IR::Namespace>
createChildren(std::multimap<std::string, std::string> const& parentToChildren,
               std::string const& parent) {
	std::vector<IR::Namespace> children;
	auto [start, end] = parentToChildren.equal_range(parent);
	for (auto it = start; it != end; it++) {
		IR::Namespace child;
		child.m_parent = it->first;
		child.m_name = it->second;
		children.emplace_back(child);
	}
	return children;
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

	// {parentName, namespaceName}
	std::multimap<std::string, std::string> parentLookup;

	std::cout << "================================" << '\n' << '\n';
	// Build parent structure and lookup tables
	for (auto& [parentName, ns] : namespaces) {
		parentLookup.insert({parentName, ns.m_name});
	}
	auto rootNS = IR::Namespace();
	rootNS.m_name = "";

	// Build child structure from top and down
	std::queue<std::reference_wrapper<IR::Namespace>> namespacesToProcess;
	namespacesToProcess.push(rootNS);
	while (!namespacesToProcess.empty()) {
		auto& currentNS = namespacesToProcess.back().get();
		namespacesToProcess.pop();

		// Create the children
		for (auto child : createChildren(parentLookup, currentNS.m_name)) {
			auto& childNS = currentNS.m_children.emplace_back(child);
			namespacesToProcess.push(childNS);
		}
	}
	std::cout << "End result:" << '\n';
	printNS(rootNS);

	// Return the root node
	return rootNS;
}
}    // namespace Helpers
