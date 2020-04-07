#include "Builders/structBuilder.hpp"
#include "Helpers/walkIRStructure.hpp"
#include "IR/ir.hpp"
#include <algorithm>
#include <clang/AST/Type.h>
#include <clang/Basic/Specifiers.h>
#include <deque>
#include <string>
#include <variant>

namespace {

void addFunctionToVariant(std::optional<IR::AccessModifier> modifier,
                          std::variant<IR::Namespace*, IR::Struct*> const& v,
                          IR::Function f) {
	if (auto ns = std::get_if<IR::Namespace*>(&v)) {
		auto& functions = (*ns)->m_functions;
		functions.push_back(f);
	} else if (auto irStruct = std::get_if<IR::Struct*>(&v)) {
		auto& functions = (*irStruct)->m_functions;
		functions.push_back({modifier.value(), f});
	}
}

IR::Function createFunction(std::string_view name,
                            IRProxy::Function const& proxyF) {
	IR::Function f;
	f.m_name = name;

	// Representation is the fully qualified name
	f.m_representation = proxyF.m_fullyQualifiedName;
	f.m_returnType = proxyF.m_returnType;
	f.m_arguments = proxyF.m_arguments;
	return f;
}

void addFunction(IRProxy::Function const& f,
                 IR::Namespace& globalNamespace) {
	// Take out the already created path
	auto path = f.m_path;
	auto [name, t] = path.back();
	path.pop_back();

	// Find the parent
	auto parentOfNewFunction =
	    Helpers::walkPathThroughStructure(path, globalNamespace);

	// Create and add the function
	addFunctionToVariant(
	    f.m_modifier, parentOfNewFunction, createFunction(name, f));
}
}    // namespace

namespace Builders {

void buildFunctions(
    const std::vector<IRProxy::Function>& functions,
    IR::Namespace& globalNamespace) {
	for (auto const& f : functions) {
		addFunction(f, globalNamespace);
	}
}

}    // namespace Builders
