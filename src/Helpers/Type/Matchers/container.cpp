#include "Helpers/Type/Matchers/container.hpp"
#include <IR/ir.hpp>
#include <ctre.hpp>
#include <optional>
#include <string_view>

namespace Helpers::Type::Matchers {

static constexpr auto vector_pattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::vector<.*?>"};

constexpr auto match_vector(std::string_view sv) noexcept {
	return ctre::match<vector_pattern>(sv);
}

static constexpr auto stack_pattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::stack<.*?>"};

constexpr auto match_stack(std::string_view sv) noexcept {
	return ctre::match<stack_pattern>(sv);
}

static constexpr auto queue_pattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::queue<.*?>"};

constexpr auto match_queue(std::string_view sv) noexcept {
	return ctre::match<queue_pattern>(sv);
}

static constexpr auto priority_queue_pattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::priority_queue<.*?>"};

constexpr auto match_priority_queue(std::string_view sv) noexcept {
	return ctre::match<priority_queue_pattern>(sv);
}

static constexpr auto deque_pattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::deque<.*?>"};

constexpr auto match_deque(std::string_view sv) noexcept {
	return ctre::match<deque_pattern>(sv);
}

static constexpr auto list_pattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::list<.*?>"};

constexpr auto match_list(std::string_view sv) noexcept {
	return ctre::match<list_pattern>(sv);
}

static constexpr auto forward_list_pattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::forward_list<.*?>"};

constexpr auto match_forward_list(std::string_view sv) noexcept {
	return ctre::match<forward_list_pattern>(sv);
}

static constexpr auto array_pattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::array<.*?>"};

constexpr auto match_array(std::string_view sv) noexcept {
	return ctre::match<array_pattern>(sv);
}

static constexpr auto map_pattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::map<.*?>"};

constexpr auto match_map(std::string_view sv) noexcept {
	return ctre::match<map_pattern>(sv);
}

static constexpr auto unordered_map_pattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::unordered_map<.*?>"};

constexpr auto match_unordered_map(std::string_view sv) noexcept {
	return ctre::match<unordered_map_pattern>(sv);
}

static constexpr auto unordered_multimap_pattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::unordered_multimap<.*?>"};

constexpr auto match_unordered_multimap(std::string_view sv) noexcept {
	return ctre::match<unordered_multimap_pattern>(sv);
}

static constexpr auto multimap_pattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::multimap<.*?>"};

constexpr auto match_multimap(std::string_view sv) noexcept {
	return ctre::match<multimap_pattern>(sv);
}

static constexpr auto multiset_pattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::multiset<.*?>"};

constexpr auto match_multiset(std::string_view sv) noexcept {
	return ctre::match<multiset_pattern>(sv);
}

static constexpr auto set_pattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::set<.*?>"};

constexpr auto match_set(std::string_view sv) noexcept {
	return ctre::match<set_pattern>(sv);
}

static constexpr auto unordered_set_pattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::unordered_set<.*?>"};

constexpr auto match_unordered_set(std::string_view sv) noexcept {
	return ctre::match<unordered_set_pattern>(sv);
}

static constexpr auto unordered_multiset_pattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::unordered_multiset<.*?>"};

constexpr auto match_unordered_multiset(std::string_view sv) noexcept {
	return ctre::match<unordered_multiset_pattern>(sv);
}

static constexpr auto allocator_pattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::allocator<.*?>"};

constexpr auto match_allocator(std::string_view sv) noexcept {
	return ctre::match<allocator_pattern>(sv);
}

static constexpr auto pair_pattern =
    ctll::fixed_string {"struct std(::__1)?::pair<.*?>"};

constexpr auto match_pair(std::string_view sv) noexcept {
	return ctre::match<pair_pattern>(sv);
}

static constexpr auto tuple_pattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::tuple<.*?>"};

constexpr auto match_tuple(std::string_view sv) noexcept {
	return ctre::match<tuple_pattern>(sv);
}

static constexpr auto greater_pattern =
    ctll::fixed_string {"struct std(::__1)?::greater<.*?>"};

constexpr auto match_greater(std::string_view sv) noexcept {
	return ctre::match<greater_pattern>(sv);
}

static constexpr auto less_pattern =
    ctll::fixed_string {"struct std(::__1)?::less<.*?>"};

constexpr auto match_less(std::string_view sv) noexcept {
	return ctre::match<less_pattern>(sv);
}

static constexpr auto hash_pattern =
    ctll::fixed_string {"struct std(::__1)?::hash<.*?>"};

constexpr auto match_hash(std::string_view sv) noexcept {
	return ctre::match<hash_pattern>(sv);
}

static constexpr auto equal_to_pattern =
    ctll::fixed_string {"struct std(::__1)?::equal_to<.*?>"};

constexpr auto match_equal_to(std::string_view sv) noexcept {
	return ctre::match<equal_to_pattern>(sv);
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
	} else if (match_deque(type)) {
		return ContainerType::Deque;
	} else if (match_less(type)) {
		return ContainerType::Less;
	} else if (match_set(type)) {
		return ContainerType::Set;
	} else if (match_map(type)) {
		return ContainerType::Map;
	} else if (match_pair(type)) {
		return ContainerType::Pair;
	} else if (match_equal_to(type)) {
		return ContainerType::EqualTo;
	} else if (match_queue(type)) {
		return ContainerType::Queue;
	} else if (match_stack(type)) {
		return ContainerType::Stack;
	} else if (match_hash(type)) {
		return ContainerType::Hash;
	} else if (match_unordered_map(type)) {
		return ContainerType::UnorderedMap;
	} else if (match_unordered_set(type)) {
		return ContainerType::UnorderedSet;
	} else if (match_tuple(type)) {
		return ContainerType::Tuple;
	} else if (match_multiset(type)) {
		return ContainerType::MultiSet;
	} else if (match_multimap(type)) {
		return ContainerType::MultiMap;
	} else if (match_list(type)) {
		return ContainerType::List;
	} else if (match_priority_queue(type)) {
		return ContainerType::PriorityQueue;
	} else if (match_unordered_multimap(type)) {
		return ContainerType::UnorderedMultiMap;
	} else if (match_unordered_multiset(type)) {
		return ContainerType::UnorderedMultiSet;
	} else if (match_forward_list(type)) {
		return ContainerType::ForwardList;
	}
	return {};
}
}    // namespace Helpers::Type::Matchers
