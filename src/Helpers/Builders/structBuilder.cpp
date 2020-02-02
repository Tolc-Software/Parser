#include "Helpers/Builders/structBuilder.h"
#include "Helpers/walkIRStructure.h"
#include "IR/ir.hpp"
#include <algorithm>
#include <deque>
#include <string>
#include <variant>

namespace {

void addStructToVariant(std::variant<IR::Namespace*, IR::Struct*> const& v,
                        IR::Struct newStruct) {
	if (auto ns = std::get_if<IR::Namespace*>(&v)) {
		auto& children = (*ns)->m_structs;
		children.push_back(newStruct);
	} else if (auto irStruct = std::get_if<IR::Struct*>(&v)) {
		auto& children = (*irStruct)->m_structs;
		children.push_back(newStruct);
	}
}

IR::Struct createStruct(std::string_view name) {
	IR::Struct newStruct;
	newStruct.m_name = name;
	return newStruct;
}

void addStruct(IRProxy::Struct& s, IR::Namespace& globalNamespace) {
	// Take out the already created path
	auto& path = s.m_name;
	auto [name, t] = path.back();
	path.pop_back();

	// Find the parent
	auto parentOfNewStruct =
	    Helpers::walkPathThroughStructure(path, globalNamespace);

	// Create and add the struct
	addStructToVariant(parentOfNewStruct, createStruct(name));
}
}    // namespace

namespace Helpers::Builders {

void buildStructs(std::vector<IRProxy::Struct>& structs,
                  IR::Namespace& globalNamespace) {
	for (auto& s : structs) {
		addStruct(s, globalNamespace);
	}
}

}    // namespace Helpers::Builders
