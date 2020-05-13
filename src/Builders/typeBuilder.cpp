#include "Builders/typeBuilder.hpp"
#include "Helpers/Type/matchers.hpp"
#include "Helpers/Type/utilities.hpp"
#include "Helpers/Utils/string.hpp"
#include "IRProxy/IRData.hpp"
#include <clang/AST/Decl.h>
#include <clang/AST/DeclTemplate.h>
#include <clang/AST/Type.h>
#include <iostream>
#include <llvm/Support/Casting.h>
#include <queue>
#include <variant>
#include <vector>

namespace Builders {

// Generate vector<QualType> with all types that class uses if it tempalated
/**
* Returns a vector of QualType's which corresponds to the inputs template arguments list
* E.g.
*   std::vector<int> -> {int, std::allocator<int>}
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

	if (auto record = type->getAsCXXRecordDecl()) {
		getTemplateArgs(record);
	}

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
		irType.m_representation =
		    Helpers::Utils::removeSubString(representation, "struct ");
		irType.m_numPointers = numPointers;
		irType.m_isConst = hasConst;
		irType.m_isReference = hasReference;
		return irType;
	}

	return {};
}

/**
* Return true if type can have a template list
*/
IR::Type* addToTemplatedList(IR::Type& type, IR::Type& toBeAdded) {
	if (auto container = std::get_if<IR::Type::Container>(&type.m_type)) {
		container->m_containedTypes.push_back(toBeAdded);
		return &container->m_containedTypes.back();
	}
	return nullptr;
}

std::optional<IR::Type> buildType(clang::QualType type) {
	std::optional<IR::Type> topLevelType = std::nullopt;
	std::queue<std::pair<IR::Type*, clang::QualType>> typesToProcess;
	if (auto irType = buildOneLevelIRType(type)) {
		topLevelType = irType.value();
		typesToProcess.push({&topLevelType.value(), type});
	} else {
		// TODO: Handle not being able to parse type
		return {};
	}

	while (!typesToProcess.empty()) {
		auto [currentType, currentQualType] = typesToProcess.front();
		// Go one step down
		for (auto qualTemplateArg : getTemplateArgs(currentQualType)) {
			// Try to convert to ir
			if (auto irType = buildOneLevelIRType(qualTemplateArg)) {
				// Push it onto the current ir
				if (auto irTemplateArg =
				        addToTemplatedList(*currentType, irType.value())) {
					// Check if we need to go even further
					if (isTemplateSpecialization(*irTemplateArg) &&
					    isTemplateSpecialization(qualTemplateArg)) {
						typesToProcess.push({irTemplateArg, qualTemplateArg});
					}
				}
			} else {
				// TODO: Handle not being able to parse type
				return {};
			}
		}
		typesToProcess.pop();
	}

	return topLevelType;
}

}    // namespace Builders
