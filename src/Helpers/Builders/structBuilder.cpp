#include "Helpers/Builders/structBuilder.h"
#include "IR/ir.hpp"
#include <algorithm>
#include <deque>
#include <string>
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

IR::Struct createStruct(Helpers::Struct& s) {
	IR::Struct newStruct;
	newStruct.m_name = s.m_name.back().first;
	return newStruct;
}

void addStructToVariant(std::variant<IR::Namespace*, IR::Struct*> const& v,
                        Helpers::Struct& s) {
	if (auto ns = std::get_if<IR::Namespace*>(&v)) {
		auto& children = (*ns)->m_structs;
		children.push_back(createStruct(s));
	} else if (auto irStruct = std::get_if<IR::Struct*>(&v)) {
		auto& children = (*irStruct)->m_structs;
		children.push_back(createStruct(s));
	}
}

void addStruct(Helpers::Struct& s, IR::Namespace& globalNamespace) {
	// Represents the current node in the globalNamespace
	std::variant<IR::Namespace*, IR::Struct*> current = &globalNamespace;
	while (!s.m_name.empty()) {
		auto [name, type] = s.m_name.front();
		s.m_name.pop_front();
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
				} else {
					// Couldn't locate the node of type struct => must be the one we should add
					addStructToVariant(current, s);
				}
				break;
			case Type::Unknown:
				// TODO: Handle unknown
				break;
		}
	}
}
}    // namespace

namespace Helpers::Builders {

void buildStructs(std::vector<Helpers::Struct>& structs,
                  IR::Namespace& globalNamespace) {
	for (auto& s : structs) {
		addStruct(s, globalNamespace);
	}
}

}    // namespace Helpers::Builders
