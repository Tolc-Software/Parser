#include "Helpers/Type/matchers.h"
#include <IR/ir.hpp>
#include <optional>
#include <string_view>

namespace Helpers::Type {

std::optional<IR::BaseType> getBaseType(std::string_view type) {
	using IR::BaseType;
	if (type == "char") {
		return BaseType::Char;
	} else if (type == "double") {
		return BaseType::Double;
	} else if (type == "float") {
		return BaseType::Float;
	} else if (type == "int") {
		return BaseType::Int;
	} else if (type == "long") {
		return BaseType::Long;
	// Either in LLVM lib or the one provided by Apple
	} else if (type == "class std::__cxx11::basic_string<char>" || type == "class std::__1::basic_string<char>") {
		return BaseType::String;
	} else if (type == "void") {
		return BaseType::Void;
	}
	return {};
}

std::optional<IR::ContainerType> getContainerType(std::string_view type) {
	using IR::ContainerType;
	if (type == "char") {
		return ContainerType::Vector;
	} else if (type == "double") {
		return ContainerType::Array;
	} else if (type == "double") {
		return ContainerType::Map;
	} else if (type == "double") {
		return ContainerType::Set;
	} else if (type == "double") {
		return ContainerType::Unordered_map;
	} else if (type == "double") {
		return ContainerType::Unordered_set;
	}
	return {};
}

}    // namespace Helpers::Type
