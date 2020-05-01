#include "Builders/typeBuilder.hpp"
#include "Helpers/Type/matchers.hpp"
#include "Helpers/Type/utilities.hpp"
#include "Helpers/Utils/string.hpp"
#include "IRProxy/IRData.hpp"
#include <clang/AST/Type.h>

namespace Builders {

std::optional<IR::Type> buildType(clang::QualType type) {
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
		irType.m_representation =
		    Helpers::Utils::removeSubString(representation, "struct ");
		irType.m_numPointers = numPointers;
		irType.m_isConst = hasConst;
		irType.m_isReference = hasReference;
		return irType;
	}

	return {};
}

}    // namespace Builders
