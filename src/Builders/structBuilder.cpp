#include "Builders/structBuilder.hpp"
#include "Helpers/walkIRStructure.hpp"
#include "IR/ir.hpp"
#include <algorithm>
#include <deque>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
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

IR::Struct createStruct(IRProxy::Struct& s) {
	IR::Struct newStruct;
	auto& path = s.m_path;
	auto& [name, structure] = path.back();

	newStruct.m_name = name;
	// Representation is the fully qualified name
	newStruct.m_representation = s.m_fullyQualifiedName;
	newStruct.m_memberVariables = s.m_variables;

	newStruct.m_hasDefaultConstructor = s.m_hasDefaultConstructor;
	return newStruct;
}

void addStruct(IRProxy::Struct& s, IR::Namespace& globalNamespace) {
	// Take out the already created path
	auto path = s.m_path;
	path.pop_back();

	// Find the parent
	auto parentOfNewStruct =
	    Helpers::walkPathThroughStructure(path, globalNamespace);

	// Create and add the struct
	addStructToVariant(parentOfNewStruct, createStruct(s));
}

std::optional<std::vector<IRProxy::MemberVariable>> getVariables(
    IRProxy::Struct const& s,
    std::unordered_map<std::string, std::vector<IRProxy::MemberVariable>>&
        memberVariables) {
	if (auto variables = memberVariables.find(s.m_fullyQualifiedName);
	    variables != memberVariables.end()) {
		return variables->second;
	}
	return {};
}

}    // namespace

namespace Builders {

void addMemberVariables(std::vector<IRProxy::Struct>& structs,
                        std::unordered_map<std::string, std::vector<IRProxy::MemberVariable>>& memberVariables) {
	for (auto& s : structs) {
		if (auto variables = getVariables(s, memberVariables)) {
			for (auto& variable : variables.value()) {
				s.m_variables.push_back({variable.m_modifier, variable.m_variable});
			}
		}
	}
}

void buildStructs(std::vector<IRProxy::Struct>& structs,
                  IR::Namespace& globalNamespace) {
	for (auto& s : structs) {
		addStruct(s, globalNamespace);
	}
}

}    // namespace Builders
