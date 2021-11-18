#include "Builders/functionBuilder.hpp"
#include "Builders/commonBuilder.hpp"
#include "Builders/structBuilder.hpp"
#include "Builders/typeBuilder.hpp"
#include "Helpers/walkIRStructure.hpp"
#include <IR/ir.hpp>
#include <algorithm>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/PrettyPrinter.h>
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
	f.m_templateArguments = proxyF.m_templateArguments;
	return f;
}

void addFunction(IRProxy::Function const& f, IR::Namespace& globalNamespace) {
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

std::pair<FunctionError, std::optional<IRProxy::Function>>
buildFunction(clang::FunctionDecl* functionDecl,
              std::function<clang::QualType(clang::QualType)>
                  getPotentiallyTemplatedType) {
	IRProxy::Function parsedFunc;
	parsedFunc.m_fullyQualifiedName = functionDecl->getQualifiedNameAsString();
	parsedFunc.m_path =
	    Builders::buildStructure(functionDecl, IRProxy::Structure::Function);

	// This is passed so that while extracting text from types it is exactly what the user wrote
	auto policy =
	    clang::PrintingPolicy(functionDecl->getASTContext().getLangOpts());

	if (auto returnType = Builders::buildType(
	        getPotentiallyTemplatedType(functionDecl->getReturnType()),
	        policy)) {
		parsedFunc.m_returnType = returnType.value();
	} else {
		return {FunctionError::ReturnType, std::nullopt};
	}

	for (auto& p : functionDecl->parameters()) {
		if (auto argType = Builders::buildType(
		        getPotentiallyTemplatedType(p->getType()), policy)) {
			IR::Variable arg;
			arg.m_name = p->getName();
			arg.m_type = argType.value();
			parsedFunc.m_arguments.push_back(arg);
		} else {
			return {FunctionError::ArgumentType, std::nullopt};
		}
	}

	// Check for access modifiers (public, private, ...)
	parsedFunc.m_modifier =
	    Builders::convertToIRAccess(functionDecl->getAccess());

	return {FunctionError::Ok, parsedFunc};
}

void buildFunctions(const std::vector<IRProxy::Function>& functions,
                    IR::Namespace& globalNamespace) {
	for (auto const& f : functions) {
		addFunction(f, globalNamespace);
	}
}

}    // namespace Builders
