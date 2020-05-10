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
#include <vector>

namespace Builders {

// generate vector<QualType> with all types that class uses if it tempalated
std::vector<clang::QualType> get_type_dependencies(
    clang::CXXRecordDecl const* C /*, bool include_members*/) {
	std::vector<clang::QualType> r;

	std::cout << "Checking for specialization" << '\n';
	if (auto t = llvm::dyn_cast<clang::ClassTemplateSpecializationDecl>(C)) {
		std::cout << "It is a class specialization!" << '\n';
		for (unsigned int i = 0; i < t->getTemplateArgs().size(); ++i) {
			if (t->getTemplateArgs()[i].getKind() ==
			    clang::TemplateArgument::Type) {
				r.push_back(
				    t->getTemplateArgs()[i]
				        .getAsType() /*.getDesugaredType(C->getASTContext())*/);
			}
		}
	}

	for (auto const& t : r) {
		std::cout << t.getAsString() << '\n';
	}

	return r;
}

std::optional<IR::Type> buildType(clang::QualType type) {
	// What the user wrote
	auto representation = type.getAsString();

	std::cout << "Processing representation: " << representation << '\n';
	if (auto record = type->getAsCXXRecordDecl()) {
		get_type_dependencies(record);
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

}    // namespace Builders
