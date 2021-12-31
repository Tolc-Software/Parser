#include "Helpers/getStructData.hpp"
#include <IR/ir.hpp>

namespace Helpers {

IR::StructData* getStructDataBasedOnAccess(IR::Struct& s,
                                           IR::AccessModifier access) {
	using IR::AccessModifier;
	switch (access) {
		case AccessModifier::Public: return &s.m_public;
		case AccessModifier::Private: return &s.m_private;
		case AccessModifier::Protected: return &s.m_protected;
	}
	return nullptr;
}

}
