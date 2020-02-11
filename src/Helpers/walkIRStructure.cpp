#include "Helpers/walkIRStructure.h"
#include "IRProxy/IRData.h"
#include <IR/ir.hpp>
#include <algorithm>
#include <variant>

namespace {

template <typename IRType>
typename std::vector<IRType>::iterator findWithName(std::vector<IRType>& values,
                                                    std::string_view name) {
	return std::find_if(values.begin(), values.end(), [&name](auto const& n) {
		return n.m_name == name;
	});
}

/*
 * Go to a Struct child of current corresponding to name
 */
IR::Struct* gotoStruct(std::variant<IR::Namespace*, IR::Struct*> const& current,
                       std::string const& name) {
	if (auto ns = std::get_if<IR::Namespace*>(&current)) {
		auto& children = (*ns)->m_structs;
		if (auto it = findWithName(children, name); it != children.end()) {
			return &(*it);
		}
	} else if (auto s = std::get_if<IR::Struct*>(&current)) {
		auto& children = (*s)->m_structs;
		if (auto it = findWithName(children, name); it != children.end()) {
			return &(*it);
		}
	}
	return nullptr;
}

/*
 * Go to a Namespace child of current corresponding to name
 */
IR::Namespace*
gotoNamespace(std::variant<IR::Namespace*, IR::Struct*> const& current,
              std::string const& name) {
	if (auto ns = std::get_if<IR::Namespace*>(&current)) {
		auto& children = (*ns)->m_namespaces;
		if (auto it = findWithName(children, name); it != children.end()) {
			return &(*it);
		}
	}
	// Structs cannot have namespaces as children
	return nullptr;
}

}    // namespace

namespace Helpers {

std::variant<IR::Namespace*, IR::Struct*> walkPathThroughStructure(
    std::deque<std::pair<std::string, IRProxy::Structure>> path,
    IR::Namespace& globalNamespace) {
	// Represents the current node in the globalNamespace
	std::variant<IR::Namespace*, IR::Struct*> current = &globalNamespace;
	while (!path.empty()) {
		auto [name, type] = path.front();
		path.pop_front();
		using IRProxy::Structure;
		switch (type) {
			case Structure::Namespace:
				if (auto next = gotoNamespace(current, name)) {
					current = next;
				}
				break;
			case Structure::Struct:
				if (auto next = gotoStruct(current, name)) {
					current = next;
				}
				break;
			case Structure::Function:
				// TODO: Handle unknown
				break;
		}
	}
	return current;
}

}    // namespace Helpers
