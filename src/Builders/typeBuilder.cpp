#include "Builders/typeBuilder.h"
#include "Helpers/Type/matchers.h"
#include "Helpers/Type/utilities.h"
#include "IRProxy/IRData.h"
#include <clang/AST/Type.h>

namespace Builders {

std::optional<IR::Type> buildType(clang::QualType type) {
	// Remove using aliases
	type = type.getCanonicalType();

	// Remove pointers
	auto numPointers = Helpers::Type::getNumberOfPointers(type);
	type = Helpers::Type::getTypeWithPointersRemoved(type);

	// Remove const
	// TODO: Handle unsupported qualifiers
	bool hasConst = Helpers::Type::isConst(type);
	type = type.getUnqualifiedType();

	if (auto maybeIrType = Helpers::Type::getIRType(type.getAsString())) {
		auto irType = maybeIrType.value();

		irType.m_numPointers = numPointers;
		irType.m_isConst = hasConst;
		return irType;
	}

	return {};
}

}    // namespace Builders
