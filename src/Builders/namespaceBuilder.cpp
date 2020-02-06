#include "Builders/namespaceBuilder.h"
#include "Helpers/Utils/split.h"
#include "IR/ir.hpp"
#include <algorithm>
#include <deque>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

/*
 * Get the child to parent corresponding to childName.
 * If it doesn't exist, create it and return the new child
 */
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

/*
 * Add the fully qualified namespace defined by the deque to the root or global namespace
 */
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
}    // namespace

namespace Builders {

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

	// Create the global namespace
	auto rootNS = IR::Namespace();
	rootNS.m_name = "";

	// Build parent structure and lookup tables
	for (auto& ns : fqNamespaces) {
		addNamespaceToRoot(rootNS, ns);
	}

	// Return the root node
	return rootNS;
}
}    // namespace Builders
