#include "Helpers/Type/utilities.hpp"
#include "Helpers/Type/matchers.hpp"
#include <IR/ir.hpp>
#include <clang/AST/Type.h>
#include <optional>
#include <string_view>

namespace Helpers::Type {

clang::QualType getTypeWithPointersRemoved(clang::QualType type) {
	while (type->isAnyPointerType()) {
		type = type->getPointeeType();
	}
	return type;
}

/**
  * Take a value and build a type
  */
IR::Type buildTypeFromValue(IR::Type::Value value) {
	IR::Type type;
	type.m_type = value;
	return type;
}

/**
* Take a user defined type and create a full type
*/
IR::Type buildTypeFromUserDefined(IR::Type::UserDefined userDefined) {
	IR::Type type;
	type.m_type = userDefined;
	return type;
}

// /**
//   * Take a container and build a type
//   */
// IR::Type buildTypeFromContainer(IR::BaseType valueType,
//                                 clang::QualType originalType) {
// 	IR::Type::Value v;
// 	v.m_valueType = valueType;

// IR::Type type;
// type.m_type = v;
// return type;
// }

/**
* Tries to create an IR::Type (Int, Void, String, ...) of the input
* If it fails it returns std::nullopt
* NOTE: Assumes the qualifiers are removed (const, ...)
*/
std::optional<IR::Type> getIRType(std::string_view type) {
	if (auto value = getValueType(type)) {
		return buildTypeFromValue(value.value());
	}
	// TODO: Support containers
	else if (auto containerType = getContainerType(type)) {
		// return buildTypeFromContainer(containerType.value());
		return {};
	} else if (auto userDefined = getUserDefinedType(type)) {
		return buildTypeFromUserDefined(userDefined.value());
	}
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
* Checks if type is const
*/
bool isConst(clang::QualType const& type) {
	return type.split().Quals.hasConst();
}

}    // namespace Helpers::Type
