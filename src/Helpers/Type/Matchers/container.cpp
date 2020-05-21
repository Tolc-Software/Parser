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

static constexpr auto array_pattern =
    ctll::fixed_string {"struct std::array<.*>"};

constexpr auto match_array(std::string_view sv) noexcept {
	return ctre::match<array_pattern>(sv);
}

static constexpr auto set_pattern = ctll::fixed_string {"class std::set<.*>"};

constexpr auto match_set(std::string_view sv) noexcept {
	return ctre::match<set_pattern>(sv);
}

static constexpr auto allocator_pattern =
    ctll::fixed_string {"class std::allocator<.*>"};

constexpr auto match_allocator(std::string_view sv) noexcept {
	return ctre::match<allocator_pattern>(sv);
}

static constexpr auto greater_pattern =
    ctll::fixed_string {"struct std::greater<.*>"};

constexpr auto match_greater(std::string_view sv) noexcept {
	return ctre::match<greater_pattern>(sv);
}

static constexpr auto less_pattern =
    ctll::fixed_string {"struct std::less<.*>"};

constexpr auto match_less(std::string_view sv) noexcept {
	return ctre::match<less_pattern>(sv);
}

std::optional<IR::ContainerType> getContainerType(std::string_view type) {
	using IR::ContainerType;
	if (match_vector(type)) {
		return ContainerType::Vector;
	} else if (match_allocator(type)) {
		return ContainerType::Allocator;
	} else if (match_array(type)) {
		return ContainerType::Array;
	} else if (match_greater(type)) {
		return ContainerType::Greater;
	} else if (match_less(type)) {
		return ContainerType::Less;
	} else if (match_set(type)) {
		return ContainerType::Set;
	} else if (type == "map") {
		return ContainerType::Map;
	} else if (type == "unordered_map") {
		return ContainerType::Unordered_map;
	} else if (type == "unordered_set") {
		return ContainerType::Unordered_set;
	}
	return {};
}
}    // namespace Helpers::Type::Matchers
