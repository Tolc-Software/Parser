#pragma once

#include "IR/ir.hpp"
#include "Parser/Parse.h"

namespace TestUtil {

std::vector<IR::Type> getTypes() {
	using IR::Type;
	return {
	    Type::Char,
	    Type::Double,
	    Type::Float,
	    Type::Int,
	    Type::Long,
	    Type::String,
	    Type::Void,
	};
}

std::string getTypeAsString(IR::Type type) {
	std::string s = "";
	using IR::Type;
	switch (type) {
		case Type::Char: s = "char"; break;
		case Type::Double: s = "double"; break;
		case Type::Float: s = "float"; break;
		case Type::Int: s = "int"; break;
		case Type::Long: s = "long"; break;
		case Type::String: s = "std::string"; break;
		case Type::Void: s = "void"; break;
	}
	return s;
}

std::string getIncludesIfNeeded(IR::Type type) {
	std::string include = "";
	using IR::Type;
	switch (type) {
		case Type::String: include = "#include <string>\n"; break;
		default: break;
	}
	return include;
}

std::string getValidReturnForType(IR::Type type) {
	std::string validReturn = "";
	using IR::Type;
	switch (type) {
		case Type::Char: validReturn = "'c'"; break;
		case Type::Double: validReturn = "1.0"; break;
		case Type::Float: validReturn = "1.0"; break;
		case Type::Int: validReturn = "1"; break;
		case Type::Long: validReturn = "1"; break;
		case Type::String: validReturn = "\"HelloWorld\""; break;
		case Type::Void: validReturn = ""; break;
	}
	return validReturn;
}

}    // namespace TestUtil
