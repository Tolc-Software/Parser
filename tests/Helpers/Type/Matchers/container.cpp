#include "Helpers/Type/Matchers/container.hpp"
#include <TestUtil/types.hpp>
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <string>

TEST_CASE("Can parse out a simple vector", "[container]") {
	using namespace Helpers::Type::Matchers;
	for (auto const& baseType :
	     TestUtil::getBaseTypes({"void"} /* excluding void */)) {
		std::string vector = fmt::format(
		    "class std::vector<{baseType}, class std::allocator<{baseType}> >",
		    fmt::arg("baseType", baseType));
		CAPTURE(vector);

		auto ir = getContainerType(vector);
		REQUIRE(ir.has_value());
		REQUIRE(ir.value() == IR::ContainerType::Vector);
	}
}

TEST_CASE("std::less and std::greater", "[container]") {
	using namespace Helpers::Type::Matchers;
	for (auto [comparer, irType] :
	     {std::make_pair("less", IR::ContainerType::Less),
	      std::make_pair("greater", IR::ContainerType::Greater)}) {
		std::string lessOrGreater = fmt::format("struct std::{comparer}<char>",
		                                        fmt::arg("comparer", comparer));
		CAPTURE(lessOrGreater);

		auto ir = getContainerType(lessOrGreater);
		REQUIRE(ir.has_value());
		REQUIRE(ir.value() == irType);
	}
}

TEST_CASE("std::set", "[container]") {
	using namespace Helpers::Type::Matchers;
	std::string set =
	    "class std::set<char, struct std::less<char>, class std::allocator<char> >";
	CAPTURE(set);

	auto ir = getContainerType(set);
	REQUIRE(ir.has_value());
	REQUIRE(ir.value() == IR::ContainerType::Set);
}

TEST_CASE("std::map", "[container]") {
	using namespace Helpers::Type::Matchers;
	std::string map =
	    "class std::map<char, char, struct std::less<char>, class std::allocator<struct std::pair<const char, char> > >";
	CAPTURE(map);

	auto ir = getContainerType(map);
	REQUIRE(ir.has_value());
	REQUIRE(ir.value() == IR::ContainerType::Map);
}

TEST_CASE("std::pair", "[container]") {
	using namespace Helpers::Type::Matchers;
	std::string pair = "struct std::pair<const char, char>";
	CAPTURE(pair);

	auto ir = getContainerType(pair);
	REQUIRE(ir.has_value());
	REQUIRE(ir.value() == IR::ContainerType::Pair);
}
