#include "Builders/structBuilder.h"
#include "Helpers/walkIRStructure.h"
#include "IR/ir.hpp"
#include <algorithm>
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
	f.m_returnType = proxyF.m_returnType;
	f.m_arguments = proxyF.m_arguments;
	return f;
}

void addFunction(std::optional<IR::AccessModifier> modifier,
                 IRProxy::Function const& f,
                 IR::Namespace& globalNamespace) {
	// Take out the already created path
	auto path = f.m_name;
	auto [name, t] = path.back();
	path.pop_back();

	// Find the parent
	auto parentOfNewFunction =
	    Helpers::walkPathThroughStructure(path, globalNamespace);

	// Create and add the function
	addFunctionToVariant(
	    modifier, parentOfNewFunction, createFunction(name, f));
}
}    // namespace

namespace Builders {

void buildFunctions(
    const std::vector<std::pair<std::optional<IR::AccessModifier>,
                                IRProxy::Function>>& functions,
    IR::Namespace& globalNamespace) {
	for (auto [modifier, f] : functions) {
		addFunction(modifier, f, globalNamespace);
	}
}

std::optional<IR::AccessModifier>
convertToIRAccess(clang::AccessSpecifier access) {
	switch (access) {
		case clang::AS_public: return IR::AccessModifier::Public;
		case clang::AS_private: return IR::AccessModifier::Private;
		case clang::AS_protected: return IR::AccessModifier::Protected;
		case clang::AS_none: return {};
	}
}

}    // namespace Builders
