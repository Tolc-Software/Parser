#include "Parser/Parse.h"
#include "catch2/catch.hpp"

TEST_CASE("Single namespace", "[namespaces]") {
	auto namespaces = Parser::parseString("namespace Test {}");
	SECTION("Parser finds an empty namespace named Test") {
		REQUIRE(namespaces.size() == 1);
		auto& testNs = namespaces[0];

		CHECK(testNs.m_constants.empty());
		CHECK(testNs.m_functions.empty());
		CHECK(testNs.m_structs.empty());

		REQUIRE(testNs.m_name == "Test");
	}
}

TEST_CASE("Two namespaces", "[namespaces]") {
	auto namespaces = Parser::parseString(R"(
namespace Test0 {}
namespace Test1 {}
		)");
	SECTION("Parser finds two empty namespaces") {
		REQUIRE(namespaces.size() == 2);
		for (auto& ns : namespaces) {
			CHECK(ns.m_constants.empty());
			CHECK(ns.m_functions.empty());
			CHECK(ns.m_structs.empty());
		}

		SECTION("Named correctly") {
			for (auto name : {"Test0", "Test1"}) {
				CAPTURE(name);
				REQUIRE(std::any_of(
				    namespaces.begin(),
				    namespaces.end(),
				    [name](auto const& ns) { return ns.m_name == name; }));
			}
		}
	}
}

