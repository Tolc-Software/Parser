#include "Builders/typeBuilder.hpp"
#include "Helpers/Type/matchers.hpp"
#include "Helpers/Type/utilities.hpp"
#include "Helpers/Utils/string.hpp"
#include "IRProxy/IRData.hpp"
#include <clang/AST/Decl.h>
#include <clang/AST/DeclTemplate.h>
#include <clang/AST/Type.h>
#include <llvm/Support/Casting.h>
#include <queue>
#include <variant>
#include <vector>

namespace Builders {

/**
* Returns a vector of QualType's which corresponds to the inputs template arguments list
* E.g.
*   std::vector<int, std::allocator<int>> -> {int, std::allocator<int>}
*/
std::vector<clang::QualType>
getTemplateArgs(clang::CXXRecordDecl const* classDecl) {
	std::vector<clang::QualType> templateArgs;

	if (auto templatedClass =
	        llvm::dyn_cast<clang::ClassTemplateSpecializationDecl>(classDecl)) {
		for (auto const& arg : templatedClass->getTemplateArgs().asArray()) {
			if (arg.getKind() == clang::TemplateArgument::Type) {
				templateArgs.push_back(arg.getAsType());
			}
		}
	}

	return templateArgs;
}

std::vector<clang::QualType> getTemplateArgs(clang::QualType type) {
	if (auto classType = type->getAsCXXRecordDecl()) {
		return getTemplateArgs(classType);
	}
	return {};
}

/**
* True if it is a template specialization. E.g. std::vector<int>
*/
bool isTemplateSpecialization(clang::QualType type) {
	if (auto classDecl = type->getAsCXXRecordDecl()) {
		return llvm::dyn_cast<clang::ClassTemplateSpecializationDecl>(
		           classDecl) != nullptr;
	}
	return false;
}

/**
* True if it is a template specialization. E.g. std::vector<int>
*/
bool isTemplateSpecialization(IR::Type const& type) {
	// TODO: This should be true for user defined templates aswell maybe?
	return std::get_if<IR::Type::Container>(&type.m_type);
}

/**
* Builds a shallow type. This does not include template arguments.
* Note that this is the complete type if the input is 'simple' as say 'int const*'
*/
std::optional<IR::Type> buildOneLevelIRType(clang::QualType type) {
	// What the user wrote
	auto representation = type.getAsString();

	// Remove using aliases
	type = type.getCanonicalType();

	// Remove pointers
	auto numPointers = Helpers::Type::getNumberOfPointers(type);
	type = Helpers::Type::getTypeWithPointersRemoved(type);

	// Remove references
	auto hasReference = type.getTypePtr()->isReferenceType();
	type = type.getNonReferenceType();

	// Remove const
	// TODO: Handle unsupported qualifiers
	bool hasConst = Helpers::Type::isConst(type);
	type = type.getUnqualifiedType();

	if (auto maybeIrType = Helpers::Type::getIRType(type.getAsString())) {
		auto irType = maybeIrType.value();

		// User defined types has a string type as {const struct UserDefined&}
		// But since we are not interested in the "struct " part, we remove it if it exists
		for (auto classPrefix : {"struct ", "class ", "enum "}) {
			representation =
			    Helpers::Utils::removeSubString(representation, classPrefix);
		}
		irType.m_representation = representation;
		irType.m_numPointers = numPointers;
		irType.m_isConst = hasConst;
		irType.m_isReference = hasReference;
		return irType;
	}

	return {};
}

/**
* Used to link an IR type with its template arguments
*/
struct ProxyType {
	IR::Type* m_type;
	std::vector<std::pair<IR::Type, clang::QualType>> m_templateArgs;
};

/**
* Builds a ProxyType that contains template arguments of irType
*/
ProxyType buildProxyType(IR::Type& irType, clang::QualType type) {
	ProxyType p;
	p.m_type = &irType;
	// Check if we need to go even further
	if (isTemplateSpecialization(irType) && isTemplateSpecialization(type)) {
		for (auto templateArg : getTemplateArgs(type)) {
			if (auto irTemplateArg = buildOneLevelIRType(templateArg)) {
				p.m_templateArgs.emplace_back(
				    std::make_pair(irTemplateArg.value(), templateArg));
			}
		}
	}
	return p;
}

/**
* Try to get the container type from type, otherwise nullptr
*/
IR::Type::Container* getContainer(IR::Type* type) {
	return std::get_if<IR::Type::Container>(&type->m_type);
}

std::optional<IR::Type> buildType(clang::QualType type) {
	std::optional<IR::Type> topLevelType = std::nullopt;
	std::queue<ProxyType> typesToProcess;
	if (auto irType = buildOneLevelIRType(type)) {
		topLevelType = irType.value();
		typesToProcess.push(buildProxyType(topLevelType.value(), type));
	}

	// Process template arguments
	while (!typesToProcess.empty()) {
		auto& currentType = typesToProcess.front();
		if (auto container = getContainer(currentType.m_type)) {
			// Is needed to avoid pointer to reallocated vector entry
			container->m_containedTypes.reserve(
			    currentType.m_templateArgs.size());

			for (auto const& [irTemplateArg, clangTemplateArg] :
			     currentType.m_templateArgs) {
				container->m_containedTypes.push_back(irTemplateArg);

				typesToProcess.push(buildProxyType(
				    container->m_containedTypes.back(), clangTemplateArg));
			}
		}

		// NOTE: This has to be at the bottom for currentType to survive this long
		typesToProcess.pop();
	}

	return topLevelType;
}

}    // namespace Builders
