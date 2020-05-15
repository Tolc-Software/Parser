#pragma once

#include "IR/ir.hpp"
#include "Parser/Parse.hpp"
#include <algorithm>
#include <string>
#include <vector>

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

std::optional<IR::BaseType> getIRFromString(std::string const& type) {
	using IR::BaseType;
	if (type == "char")
		return BaseType::Char;
	if (type == "double")
		return BaseType::Double;
	if (type == "float")
		return BaseType::Float;
	if (type == "int")
		return BaseType::Int;
	if (type == "long")
		return BaseType::Long;
	if (type == "std::string")
		return BaseType::String;
	if (type == "void")
		return BaseType::Void;
	return {};
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
		case BaseType::Double:
		case BaseType::Float:
		case BaseType::Int:
		case BaseType::Long: return "0";
		case BaseType::Char: return "'c'";
		case BaseType::String: return "\"HelloWorld\"";
		case BaseType::Void: return "";
	}
	return "";
}

/**
* Return all (hopefully) valid base types as strings. Optionally exclude some by input
*/
std::vector<std::string>
getBaseTypes(std::vector<std::string> const& excluding = {}) {
	std::vector<std::string> baseTypes = {
	    "char", "double", "float", "int", "long", "std::string", "void"};
	for (auto e : excluding) {
		baseTypes.erase(std::remove(baseTypes.begin(), baseTypes.end(), e),
		                baseTypes.end());
	}
	return baseTypes;
}

}    // namespace TestUtil
