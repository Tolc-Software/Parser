#include "Helpers/getStructData.hpp"
#include "IRProxy/IRData.hpp"
#include <IR/ir.hpp>

namespace Helpers {

IR::StructData* getStructDataBasedOnAccess(IR::Struct& s,
                                           IRProxy::AccessModifier access) {
	using IRProxy::AccessModifier;
	switch (access) {
		case AccessModifier::Public: return &s.m_public;
		case AccessModifier::Private: return &s.m_private;
		case AccessModifier::Protected: return &s.m_protected;
	}
	return nullptr;
}
}
