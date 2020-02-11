#pragma once

#include "IR/ir.hpp"
#include "Parser/Parse.h"

namespace TestUtil {

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

std::string getTypeAsString(IR::BaseType type) {
	std::string s = "";
	using IR::BaseType;
	switch (type) {
		case BaseType::Char: s = "char"; break;
		case BaseType::Double: s = "double"; break;
		case BaseType::Float: s = "float"; break;
		case BaseType::Int: s = "int"; break;
		case BaseType::Long: s = "long"; break;
		case BaseType::String: s = "std::string"; break;
		case BaseType::Void: s = "void"; break;
	}
	return s;
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
