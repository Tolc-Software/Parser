#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"
#include <catch2/catch.hpp>

TEST_CASE("Constructor and destructor", "[constructorsDestructors]") {
	auto globalNS = TestUtil::parseString(R"(
struct Constructor {
	explicit Constructor();
	explicit Constructor(int);

	~Constructor();
};
)");

	auto& constructor = TestUtil::findStruct(globalNS, "Constructor");
	REQUIRE(constructor.m_constructors.size() == 2);
	REQUIRE(constructor.m_destructors.size() == 1);
	REQUIRE(constructor.m_functions.empty());
}

