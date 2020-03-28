#include "TestUtil/parse.hpp"
#include <catch2/catch.hpp>

TEST_CASE("A simple user defined struct", "[userDefined]") {
	REQUIRE(1 == 1);
	auto globalNS = TestUtil::parseString(R"(
struct Simple {};

Simple f();
)");
	SECTION("Parser finds an empty struct named Simple") {
		REQUIRE(globalNS.m_functions.size() == 1);
		auto& f = globalNS.m_functions[0];
		REQUIRE(f.m_name == "f");
	}
}
