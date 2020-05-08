#include "Helpers/Type/Matchers/container.hpp"
#include <IR/ir.hpp>
#include <ctre.hpp>
#include <optional>
#include <string_view>

namespace Helpers::Type::Matchers {

static constexpr auto vector_pattern =
    ctll::fixed_string {"class std::vector<.*>"};

constexpr auto match_vector(std::string_view sv) noexcept {
	return ctre::match<vector_pattern>(sv);
}

std::optional<IR::ContainerType> getContainerType(std::string_view type) {
	using IR::ContainerType;
	if (match_vector(type)) {
		return ContainerType::Vector;
	} else if (type == "array") {
		return ContainerType::Array;
	} else if (type == "map") {
		return ContainerType::Map;
	} else if (type == "set") {
		return ContainerType::Set;
	} else if (type == "unordered_map") {
		return ContainerType::Unordered_map;
	} else if (type == "unordered_set") {
		return ContainerType::Unordered_set;
	}
	return {};
}
}    // namespace Helpers::Type::Matchers
