#include "Builders/typeBuilder.h"
#include "IRProxy/IRData.h"
#include <clang/AST/DeclBase.h>
#include <clang/AST/Type.h>
#include <deque>
#include <optional>
#include <string>
#include <string_view>

namespace {

std::optional<IR::BaseType> getBaseType(std::string_view type) {
	using IR::BaseType;
	if (type == "char") {
		return BaseType::Char;
	} else if (type == "double") {
		return BaseType::Double;
	} else if (type == "float") {
		return BaseType::Float;
	} else if (type == "int") {
		return BaseType::Int;
	} else if (type == "long") {
		return BaseType::Long;
	} else if (type == "string") {
		return BaseType::String;
	} else if (type == "void") {
		return BaseType::Void;
	}
	return {};
}

clang::QualType getTypeWithPointersRemoved(clang::QualType type) {
	while (type->isAnyPointerType()) {
		type = type->getPointeeType();
	}
	return type;
}

/**
  * Take a base and build a type
  */
IR::Type buildTypeFromBase(IR::BaseType valueType,
                           std::optional<IR::BaseType> keyType = std::nullopt) {
	IR::Type::Value v;
	v.m_valueType = valueType;
	v.m_keyType = keyType;

	IR::Type type;
	type.m_type = v;
	return type;
}

/**
* Tries to create an IR::Type (Int, Void, String, ...) of the input
* If it fails it returns std::nullopt
* NOTE: Assumes the qualifiers are removed (const, ...)
*/
std::optional<IR::Type> getIRType(std::string_view type) {
	if (auto baseType = getBaseType(type)) {
		return buildTypeFromBase(baseType.value());
	}
	// TODO: Support containers
	// else if (auto containerType = getContainerType(type)) {
	// return buildTypeFromContainer(containerType.value());
	// }
	return {};
}

int getNumberOfPointers(clang::QualType type) {
	int numPointers = 0;
	while (type->isAnyPointerType()) {
		numPointers++;
		type = type->getPointeeType();
	}
	return numPointers;
}

/**
* Tries to create an IR::Qualifier (Const, ...) of the input
* If there are no qualifiers or if it fails it returns an empty vector
* NOTE: Assumes the qualifiers are removed (const, ...)
*/
std::vector<IR::Qualifier> getQualifiers(clang::QualType const& type) {
	std::vector<IR::Qualifier> quals;
	auto qs = type.split().Quals;
	if (qs.hasConst()) {
		quals.push_back(IR::Qualifier::Const);
	}
	return quals;
}

}    // namespace

namespace Builders {

std::optional<IR::Type> buildType(clang::QualType type) {
	// Remove pointers
	auto numPointers = getNumberOfPointers(type);
	type = getTypeWithPointersRemoved(type);

	if (auto maybeIrType = getIRType(type.getUnqualifiedType().getAsString())) {
		auto irType = maybeIrType.value();
		irType.m_numPointers = numPointers;

		// TODO: Handle unsupported qualifiers
		//       (has qualifiers but this function returns none)
		auto qualifiers = getQualifiers(type);
		irType.m_qualifiers = qualifiers;
		return irType;
	}

	return {};
}

}    // namespace Builders
