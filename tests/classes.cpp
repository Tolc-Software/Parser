#include "Parser/Parse.h"
#include <catch2/catch.hpp>

TEST_CASE("Finds a global class", "[classes]") {
	auto globalNS = Parser::parseString("class Simple {}; ");
	SECTION("Parser finds an empty class named Simple") {
		REQUIRE(globalNS.m_structs.size() == 1);
		auto& simple = globalNS.m_structs[0];
		REQUIRE(simple.m_name == "Simple");
	}
}

TEST_CASE("Finds a global struct", "[classes]") {
	auto globalNS = Parser::parseString("struct Simple {}; ");
	SECTION("Parser finds an empty struct named Simple") {
		REQUIRE(globalNS.m_structs.size() == 1);
		auto& simple = globalNS.m_structs[0];
		REQUIRE(simple.m_name == "Simple");
	}
}

TEST_CASE("Finds a class within a namespace", "[classes]") {
	auto globalNS = Parser::parseString(R"(
namespace NS {
	class Simple {};
}
		)");
	SECTION("Parser finds an empty namespace named Simple") {
		REQUIRE(globalNS.m_children.size() == 1);
		auto& ns = globalNS.m_children[0];
		CHECK(ns.m_name == "NS");
		REQUIRE(ns.m_structs.size() == 1);
		auto& simple = ns.m_structs[0];
		REQUIRE(simple.m_name == "Simple");
	}
}

TEST_CASE("Finds a struct within a struct", "[classes]") {
	auto globalNS = Parser::parseString(R"(
struct Outer {
	struct Inner {};
};
		)");
	SECTION("Parser finds the Outer struct") {
		REQUIRE(globalNS.m_structs.size() == 1);
		auto& outer = globalNS.m_structs[0];
		REQUIRE(outer.m_name == "Outer");
		SECTION("Parser finds the Inner struct") {
			REQUIRE(outer.m_structs.size() == 1);
			auto& inner = outer.m_structs[0];
			REQUIRE(inner.m_name == "Inner");
		}
	}
}