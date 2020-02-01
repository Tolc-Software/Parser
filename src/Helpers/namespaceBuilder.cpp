#include "Helpers/namespaceBuilder.h"
#include "Helpers/Utils/split.h"
#include "Helpers/namespaceHelpers.h"
#include "IR/ir.hpp"
#include <algorithm>
#include <deque>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

IR::Namespace* createOrGetChild(IR::Namespace* parent,
                                std::string_view childName) {
	auto& children = parent->m_children;
	// Check if the child exists
	if (auto child = std::find_if(children.begin(),
	                              children.end(),
	                              [&childName](auto const& childNS) {
		                              return childNS.m_name == childName;
	                              });
	    child != children.end()) {
		return &(*child);
	}
	// Create the child
	IR::Namespace childNS;
	childNS.m_name = childName;
	return &children.emplace_back(childNS);
}

void addNamespaceToRoot(IR::Namespace& rootNS, std::deque<std::string> ns) {
	auto* currentNs = &rootNS;
	while (!ns.empty()) {
		auto& name = ns.front();
		auto parentName = currentNs->m_name;
		// Set child
		currentNs = createOrGetChild(currentNs, name);
		// Set parent
		currentNs->m_parent = parentName;
		ns.pop_front();
	}
}

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
}    // namespace

namespace Helpers {

// std::string getParentNamespaceName(clang::NamespaceDecl const* namespaceDecl) {
// 	auto parentDecl = namespaceDecl->getParent();
// 	if (parentDecl->isNamespace() && !parentDecl->isStdNamespace()) {
// 		auto parentNS = static_cast<clang::NamespaceDecl const*>(parentDecl);
// 		return parentNS->getName();
// 	}
// 	return "";
// }

IR::Namespace
buildNamespaceStructure(std::vector<std::string> const& namespaces) {
	// {fully qualified namespaces divided into names}
	// Ex:
	//   Helpers::buildNamespaceStructure -> {deque({Helpers, buildNamespaceStructure})}
	std::vector<std::deque<std::string>> fqNamespaces;

	// Build the fully qualified namespaces
	for (auto& name : namespaces) {
		fqNamespaces.push_back(Helpers::Utils::split(name, "::"));
	}

	// Sort them by length (furthest from global ns)
	std::sort(
	    fqNamespaces.begin(),
	    fqNamespaces.end(),
	    [](auto const& d0, auto const& d1) { return d0.size() < d1.size(); });

	std::cout << "================================" << '\n' << '\n';
	// Create the global namespace
	auto rootNS = IR::Namespace();
	rootNS.m_name = "";

	// Build parent structure and lookup tables
	for (auto& ns : fqNamespaces) {
		addNamespaceToRoot(rootNS, ns);
	}

	// Build child structure from top and down

	std::cout << "End result:" << '\n';
	printNS(rootNS);

	// Return the root node
	return rootNS;
}
}    // namespace Helpers
