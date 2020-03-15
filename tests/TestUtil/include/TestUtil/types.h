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
		case AccessModifier::Public: return "public";
		case AccessModifier::Private: return "private";
		case AccessModifier::Protected: return "protected";
	}
	return "";
}

std::string getAsString(IR::BaseType type) {
	using IR::BaseType;
	switch (type) {
		case BaseType::Char: return "char";
		case BaseType::Double: return "double";
		case BaseType::Float: return "float";
		case BaseType::Int: return "int";
		case BaseType::Long: return "long";
		case BaseType::String: return "std::string";
		case BaseType::Void: return "void";
	}
	return "";
}

std::string getIncludesIfNeeded(IR::BaseType type) {
	std::string include = "";
	using IR::BaseType;
	switch (type) {
		case BaseType::String: include = "#include <string>\n";
		default: break;
	}
	return include;
}

std::string getValidReturnForType(IR::BaseType type) {
	std::string validReturn = "";
	using IR::BaseType;
	switch (type) {
		case BaseType::Char: return "'c'";
		case BaseType::Double: return "1.0";
		case BaseType::Float: return "1.0";
		case BaseType::Int: return "1";
		case BaseType::Long: return "1";
		case BaseType::String: return "\"HelloWorld\"";
		case BaseType::Void: return "";
	}
	return "";
}

}    // namespace TestUtil
