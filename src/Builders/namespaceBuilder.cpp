#include "Builders/namespaceBuilder.hpp"
#include "Helpers/Utils/split.hpp"
#include "IR/ir.hpp"
#include <algorithm>
#include <deque>
#include <map>
#include <queue>
#include <string>
#include <string_view>
#include <vector>

namespace {

/*
 * Get the child to parent corresponding to childName.
 * If it doesn't exist, create it and return the new child
 */
IR::Namespace* createOrGetChild(IR::Namespace* parent,
                                std::string_view childName,
                                std::string_view childRepresentation) {
	auto& children = parent->m_namespaces;
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
	childNS.m_representation = childRepresentation;
	return &children.emplace_back(childNS);
}

/*
 * Add the fully qualified namespace defined by the deque to the root or global namespace
 */
void addNamespaceToRoot(IR::Namespace& rootNS,
                        std::deque<std::string> ns,
                        std::string const& representation) {
	auto* currentNs = &rootNS;
	while (!ns.empty()) {
		auto& name = ns.front();
		auto parentName = currentNs->m_name;
		// Set child
		currentNs = createOrGetChild(currentNs, name, representation);
		ns.pop_front();
	}
}
}    // namespace

namespace Builders {

void addGlobalVariables(
    std::map<std::string, std::vector<IR::Variable>> const& variables,
    IR::Namespace& globalNS) {
	std::queue<IR::Namespace*> namespaces;
	namespaces.push(&globalNS);
	while (!namespaces.empty()) {
		auto& current = namespaces.front();
		if (auto variable = variables.find(current->m_representation);
		    variable != variables.end()) {
			current->m_variables = variable->second;
		}

		for (auto& nested : current->m_namespaces) {
			namespaces.push(&nested);
		}
		namespaces.pop();
	}
}

IR::Namespace
buildNamespaceStructure(std::vector<std::string> const& namespaces) {
	// {fully qualified namespaces divided into names}
	// Ex:
	//   Helpers::buildNamespaceStructure -> {deque({Helpers, buildNamespaceStructure}), Helpers::buildNamespaceStructure}
	std::vector<std::pair<std::deque<std::string>, std::string>> fqNamespaces;

	// Build the fully qualified namespaces
	for (auto const& name : namespaces) {
		fqNamespaces.emplace_back(Helpers::Utils::split(name, "::"), name);
	}

	// Sort them by length (deepest from global ns)
	// NOTE:
	//     Here ns0::ns1 is represented {ns0, ns1}
	std::sort(fqNamespaces.begin(),
	          fqNamespaces.end(),
	          [](auto const& ns0, auto const& ns1) {
		          return ns0.first.size() < ns1.first.size();
	          });

	// Create the global namespace
	auto rootNS = IR::Namespace();
	rootNS.m_name = "";

	// Build parent structure and lookup tables
	for (auto& [ns, representation] : fqNamespaces) {
		addNamespaceToRoot(rootNS, ns, representation);
	}

	// Return the root node
	return rootNS;
}
}    // namespace Builders
