#include "Helpers/Type/matchers.hpp"
#include <IR/ir.hpp>
#include <array>
#include <optional>
#include <string>
#include <string_view>

namespace Helpers::Type {

std::optional<IR::BaseType> getBaseType(std::string_view type) {
	using IR::BaseType;
	// Types and alternate types are listed in https://en.cppreference.com/w/cpp/language/types
	if (type == "bool" || type == "_Bool") {
		return BaseType::Bool;
	} else if (type == "char16_t") {
		return BaseType::Char16_t;
	} else if (type == "char32_t") {
		return BaseType::Char32_t;
	} else if (type == "signed char") {
		return BaseType::SignedChar;
	} else if (type == "unsigned char") {
		return BaseType::UnsignedChar;
	} else if (type == "wchar_t") {
		return BaseType::Wchar_t;
	} else if (type == "char") {
		return BaseType::Char;
	} else if (type == "double") {
		return BaseType::Double;
	} else if (type == "long double") {
		return BaseType::LongDouble;
	} else if (type == "float") {
		return BaseType::Float;
	} else if (type == "int" || type == "signed int" || type == "signed") {
		return BaseType::Int;
	} else if (type == "long" || type == "long int" || type == "signed long" ||
	           type == "signed long int") {
		return BaseType::LongInt;
	} else if (type == "long long" || type == "long long int" ||
	           type == "signed long long" || type == "signed long long int") {
		return BaseType::LongLongInt;
	} else if (type == "short" || type == "short int" ||
	           type == "signed short" || type == "signed short int") {
		return BaseType::ShortInt;
	} else if (type == "unsigned long" || type == "unsigned long int") {
		return BaseType::UnsignedLongInt;
	} else if (type == "unsigned long long" ||
	           type == "unsigned long long int") {
		return BaseType::UnsignedLongLongInt;
	} else if (type == "unsigned short" || type == "unsigned short int") {
		return BaseType::UnsignedShortInt;
	} else if (type == "unsigned" || type == "unsigned int") {
		return BaseType::UnsignedInt;
		// Either in LLVM lib, the one provided by Apple, or on Windows
	} else if (
	    type == "class std::__cxx11::basic_string<char>" ||
	    type ==
	        "class std::__1::basic_string<char, struct std::__1::char_traits<char>, class std::__1::allocator<char> >" ||
	    type ==
	        "class std::basic_string<char, struct std::char_traits<char>, class std::allocator<char> >") {
		return BaseType::String;
	} else if (type == "void") {
		return BaseType::Void;
	}
	return {};
}

std::optional<std::string> removeDeclaration(std::string_view type,
                                             std::string_view declaration) {
	if (type.size() > declaration.size()) {
		// Check for our struct declaration
		if (type.substr(0, declaration.size()) == declaration) {
			// Remove it and return the rest
			return std::string(type.substr(declaration.size()));
		}
	}
	return {};
}

std::optional<std::string> removeEnumDeclaration(std::string_view type) {
	constexpr static std::string_view enumDeclaration = "enum ";
	return removeDeclaration(type, enumDeclaration);
}

std::optional<std::string>
removeClassOrStructDeclaration(std::string_view type) {
	constexpr static std::array<std::string_view, 2> declarations = {"struct ",
	                                                                 "class "};
	for (auto const& declaration : declarations) {
		if (auto res = removeDeclaration(type, declaration)) {
			return res.value();
		}
	}
	return {};
}

std::optional<IR::Type::Value> getValueType(std::string_view type) {
	if (auto base = getBaseType(type)) {
		IR::Type::Value v;
		v.m_base = base.value();
		return v;
	}
	return {};
}

std::optional<IR::Type::UserDefined> getUserDefinedType(std::string_view type) {
	// All user defined are given as "struct FullyQualifiedNameOfClass"
	if (auto userDefined = removeClassOrStructDeclaration(type)) {
		IR::Type::UserDefined ud;
		ud.m_representation = userDefined.value();
		return ud;
	}
	return {};
}

std::optional<IR::Type::EnumValue> getEnumType(std::string_view type) {
	// All user defined are given as "struct FullyQualifiedNameOfClass"
	if (auto e = removeEnumDeclaration(type)) {
		IR::Type::EnumValue ev;
		ev.m_representation = e.value();
		return ev;
	}
	return {};
}

}    // namespace Helpers::Type
