#include "Helpers/Type/utilities.h"
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
  * Take a base and build a type
  */
IR::Type buildTypeFromBase(IR::BaseType valueType,
                           std::optional<IR::BaseType> keyType) {
	IR::Type::Value v;
	v.m_valueType = valueType;
	v.m_keyType = keyType;

	IR::Type type;
	type.m_type = v;
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
	if (auto baseType = getBaseType(type)) {
		return buildTypeFromBase(baseType.value());
	}
	// TODO: Support containers
	else if (auto containerType = getContainerType(type)) {
		return {};
		// return buildTypeFromContainer(containerType.value());
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
