#include "Parser/Parse.h"
#include "catch2/catch.hpp"

TEST_CASE("Single namespace", "[namespaces]") {
	auto namespaces = Parser::parseString("namespace Test {}");
	SECTION("Parser finds an empty namespace named Test") {
		REQUIRE(namespaces.size() == 1);
		auto& testNs = namespaces[0];

		REQUIRE(testNs.m_constants.empty());
		REQUIRE(testNs.m_functions.empty());
		REQUIRE(testNs.m_structs.empty());

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
			REQUIRE(ns.m_constants.empty());
			REQUIRE(ns.m_functions.empty());
			REQUIRE(ns.m_structs.empty());
		}

		SECTION("Named correctly") {
			for (auto name : {"Test0", "Test1"}) {
				// INFO("Trying to find name: " + name);
				REQUIRE(std::any_of(
				    namespaces.begin(),
				    namespaces.end(),
				    [name](auto const& ns) { return ns.m_name == name; }));
			}
		}
	}
}

