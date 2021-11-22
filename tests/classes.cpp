#include "TestUtil/parse.hpp"
#include <catch2/catch.hpp>

TEST_CASE("Deeply nested classes", "[classes]") {
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
	REQUIRE(globalNS.m_structs.size() == 1);
	auto& simple = globalNS.m_structs[0];
	REQUIRE(simple.m_name == "Simple");
	REQUIRE(simple.m_functions.size() == 1);
	// NOTE: It has a default constructor manually written
	REQUIRE(!simple.m_hasImplicitDefaultConstructor);
	auto [access, constructor] = simple.m_functions.back();
	REQUIRE(constructor.m_name == "Simple");
	REQUIRE(access == IR::AccessModifier::Public);
    }

TEST_CASE("Finds a global class", "[classes]") {
	for (std::string structure : {"class", "struct"}) {
		auto globalNS = TestUtil::parseString(structure + " Simple {}; ");
		REQUIRE(globalNS.m_structs.size() == 1);
		auto& simple = globalNS.m_structs[0];
		REQUIRE(simple.m_hasImplicitDefaultConstructor);
		REQUIRE(simple.m_name == "Simple");
	}
}

TEST_CASE("Finds a class within a namespace", "[classes]") {
	auto globalNS = TestUtil::parseString(R"(
namespace NS {
	class Simple {};
}
		)");
	REQUIRE(globalNS.m_namespaces.size() == 1);
	auto& ns = globalNS.m_namespaces[0];
	CHECK(ns.m_name == "NS");
	REQUIRE(ns.m_structs.size() == 1);
	auto& simple = ns.m_structs[0];
	REQUIRE(simple.m_name == "Simple");
}

TEST_CASE("Finds a struct within a struct", "[classes]") {
	auto globalNS = TestUtil::parseString(R"(
struct Outer {
	struct Inner {};
};
		)");
	REQUIRE(globalNS.m_structs.size() == 1);
	auto& outer = globalNS.m_structs[0];
	REQUIRE(outer.m_name == "Outer");
	REQUIRE(outer.m_structs.size() == 1);
	auto& inner = outer.m_structs[0];
	REQUIRE(inner.m_name == "Inner");
}
