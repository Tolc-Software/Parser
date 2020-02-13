#pragma once

#include "IR/ir.hpp"
#include "Parser/Parse.h"

namespace TestUtil {

std::vector<IR::AccessModifier> getAccessModifiers() {
	using IR::AccessModifier;
	return {
	    AccessModifier::Public,
	    AccessModifier::Private,
	    AccessModifier::Protected,
	};
}

std::vector<IR::BaseType> getTypes() {
	using IR::BaseType;
	return {
	    BaseType::Char,
	    BaseType::Double,
	    BaseType::Float,
	    BaseType::Int,
	    BaseType::Long,
	    BaseType::String,
	    BaseType::Void,
	};
}

std::string getAsString(IR::AccessModifier am) {
	using IR::AccessModifier;
	switch (am) {
		case AccessModifier::Public: return "public"; break;
		case AccessModifier::Private: return "private"; break;
		case AccessModifier::Protected: return "protected"; break;
	}
	return "";
}

std::string getAsString(IR::BaseType type) {
	using IR::BaseType;
	switch (type) {
		case BaseType::Char: return "char"; break;
		case BaseType::Double: return "double"; break;
		case BaseType::Float: return "float"; break;
		case BaseType::Int: return "int"; break;
		case BaseType::Long: return "long"; break;
		case BaseType::String: return "std::string"; break;
		case BaseType::Void: return "void"; break;
	}
	return "";
}

std::string getIncludesIfNeeded(IR::BaseType type) {
	std::string include = "";
	using IR::BaseType;
	switch (type) {
		case BaseType::String: include = "#include <string>\n"; break;
		default: break;
	}
	return include;
}

std::string getValidReturnForType(IR::BaseType type) {
	std::string validReturn = "";
	using IR::BaseType;
	switch (type) {
		case BaseType::Char: validReturn = "'c'"; break;
		case BaseType::Double: validReturn = "1.0"; break;
		case BaseType::Float: validReturn = "1.0"; break;
		case BaseType::Int: validReturn = "1"; break;
		case BaseType::Long: validReturn = "1"; break;
		case BaseType::String: validReturn = "\"HelloWorld\""; break;
		case BaseType::Void: validReturn = ""; break;
	}
	return validReturn;
}

}    // namespace TestUtil
