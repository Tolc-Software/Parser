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
