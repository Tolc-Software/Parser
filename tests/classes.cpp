#include "TestUtil/parse.hpp"
#include <catch2/catch.hpp>

TEST_CASE("Deeply nested classes", "[namespaces]") {
	auto code = R"(
#include <string>

namespace MyLib {

	class MyLibClass {
	};

	namespace Deeper {
		class MyDeeperClass {
		};
	}
}
)";
	CAPTURE(code);
	auto globalNS = TestUtil::parseString(code);
	REQUIRE(globalNS.m_namespaces.size() == 1);
	auto& myLib = globalNS.m_namespaces.back();
	REQUIRE(myLib.m_structs.size() == 1);
	REQUIRE(myLib.m_structs.back().m_name == "MyLibClass");
	REQUIRE(myLib.m_namespaces.size() == 1);
	auto& deeper = myLib.m_namespaces.back();
	REQUIRE(deeper.m_structs.size() == 1);
	REQUIRE(deeper.m_structs.back().m_name == "MyDeeperClass");
}

TEST_CASE("A class with a constructor", "[classes]") {
	auto globalNS = TestUtil::parseString(R"(
class Simple {
public:
	Simple();
};
		)");
	SECTION("Parser finds an empty namespace named Simple") {
		REQUIRE(globalNS.m_structs.size() == 1);
		auto& simple = globalNS.m_structs[0];
		REQUIRE(simple.m_name == "Simple");
		REQUIRE(simple.m_functions.size() == 1);
		auto [access, constructor] = simple.m_functions.back();
		REQUIRE(constructor.m_name == "Simple");
		REQUIRE(access == IR::AccessModifier::Public);
	}
}

TEST_CASE("Finds a global class", "[classes]") {
	auto globalNS = TestUtil::parseString("class Simple {}; ");
	SECTION("Parser finds an empty class named Simple") {
		REQUIRE(globalNS.m_structs.size() == 1);
		auto& simple = globalNS.m_structs[0];
		REQUIRE(simple.m_name == "Simple");
	}
}

TEST_CASE("Finds a global struct", "[classes]") {
	auto globalNS = TestUtil::parseString("struct Simple {}; ");
	SECTION("Parser finds an empty struct named Simple") {
		REQUIRE(globalNS.m_structs.size() == 1);
		auto& simple = globalNS.m_structs[0];
		REQUIRE(simple.m_name == "Simple");
	}
}

TEST_CASE("Finds a class within a namespace", "[classes]") {
	auto globalNS = TestUtil::parseString(R"(
namespace NS {
	class Simple {};
}
		)");
	SECTION("Parser finds an empty namespace named Simple") {
		REQUIRE(globalNS.m_namespaces.size() == 1);
		auto& ns = globalNS.m_namespaces[0];
		CHECK(ns.m_name == "NS");
		REQUIRE(ns.m_structs.size() == 1);
		auto& simple = ns.m_structs[0];
		REQUIRE(simple.m_name == "Simple");
	}
}

TEST_CASE("Finds a struct within a struct", "[classes]") {
	auto globalNS = TestUtil::parseString(R"(
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
