#include "Helpers/walkIRStructure.h"
#include "Helpers/IRData.h"
#include <IR/ir.hpp>
#include <algorithm>
#include <variant>

namespace {

IR::Struct* gotoStruct(std::variant<IR::Namespace*, IR::Struct*> const& current,
                       std::string const& name) {
	if (auto ns = std::get_if<IR::Namespace*>(&current)) {
		auto& children = (*ns)->m_structs;
		auto it =
		    std::find_if(children.begin(),
		                 children.end(),
		                 [&name](auto const& n) { return n.m_name == name; });
		return &(*it);
	} else if (auto s = std::get_if<IR::Struct*>(&current)) {
		auto& children = (*s)->m_structs;
		auto it =
		    std::find_if(children.begin(),
		                 children.end(),
		                 [&name](auto const& n) { return n.m_name == name; });
		return &(*it);
	}
	return nullptr;
}

IR::Namespace*
gotoNamespace(std::variant<IR::Namespace*, IR::Struct*> const& current,
              std::string const& name) {
	if (auto ns = std::get_if<IR::Namespace*>(&current)) {
		auto& children = (*ns)->m_children;
		auto it =
		    std::find_if(children.begin(),
		                 children.end(),
		                 [&name](auto const& n) { return n.m_name == name; });
		return &(*it);
	}
	// Structs cannot have namespaces as children
	return nullptr;
}
}    // namespace
namespace Helpers {

std::variant<IR::Namespace*, IR::Struct*>
walkPathThroughStructure(std::deque<std::pair<std::string, Helpers::Type>> path,
                         IR::Namespace& globalNamespace) {
	// Represents the current node in the globalNamespace
	std::variant<IR::Namespace*, IR::Struct*> current = &globalNamespace;
	while (!path.empty()) {
		auto [name, type] = path.front();
		path.pop_front();
		using Helpers::Type;
		switch (type) {
			case Type::Namespace:
				if (auto next = gotoNamespace(current, name)) {
					current = next;
				}
				break;
			case Type::Struct:
				if (auto next = gotoStruct(current, name)) {
					current = next;
				}
				break;
			case Type::Unknown:
				// TODO: Handle unknown
				break;
		}
	}
	return current;
}

}    // namespace Helpers
