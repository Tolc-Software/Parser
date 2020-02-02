#include "Helpers/Builders/structBuilder.h"
#include "Helpers/walkIRStructure.h"
#include "IR/ir.hpp"
#include <algorithm>
#include <deque>
#include <string>
#include <variant>

namespace {

void addFunctionToVariant(std::variant<IR::Namespace*, IR::Struct*> const& v,
                          IR::Function f) {
	if (auto ns = std::get_if<IR::Namespace*>(&v)) {
		auto& functions = (*ns)->m_functions;
		functions.push_back(f);
	} else if (auto irStruct = std::get_if<IR::Struct*>(&v)) {
		auto& functions = (*irStruct)->m_functions;
		functions.push_back(f);
	}
}

IR::Function createFunction(std::string_view name) {
	IR::Function f;
	f.m_name = name;
	return f;
}

void addFunction(IRProxy::Function& f, IR::Namespace& globalNamespace) {
	// Take out the already created path
	auto& path = f.m_name;
	auto [name, t] = path.back();
	path.pop_back();

	// Find the parent
	auto parentOfNewFunction =
	    Helpers::walkPathThroughStructure(path, globalNamespace);

	// Create and add the function
	addFunctionToVariant(parentOfNewFunction, createFunction(name));
}
}    // namespace

namespace Helpers::Builders {

void buildFunctions(std::vector<IRProxy::Function>& functions,
                    IR::Namespace& globalNamespace) {
	for (auto& f : functions) {
		addFunction(f, globalNamespace);
	}
}

}    // namespace Helpers::Builders
