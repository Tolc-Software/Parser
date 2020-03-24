#include "TestUtil/files.h"
#include "Parser/Parse.h"
#include "TestUtil/compare.h"
#include <catch2/catch.hpp>

TEST_CASE("Simple namespace", "[files]") {
	auto file = TestUtil::writeToFile(R"(
namespace Simple {}
)");
	auto globalNS = Parser::parseFile(file);
	REQUIRE(globalNS.m_namespaces.size() == 1);
	auto& testNs = globalNS.m_namespaces[0];

	REQUIRE(testNs.m_name == "Simple");
}

TEST_CASE("Simple struct", "[files]") {
	auto file = TestUtil::writeToFile(R"(
class Simple {};
)");
	auto globalNS = Parser::parseFile(file);
	REQUIRE(globalNS.m_structs.size() == 1);
	auto& simple = globalNS.m_structs[0];
	REQUIRE(simple.m_name == "Simple");
}

TEST_CASE("Simple function", "[files]") {
	auto file = TestUtil::writeToFile(R"(
void fun();
)");
	auto globalNS = Parser::parseFile(file);
	REQUIRE(globalNS.m_functions.size() == 1);
	auto fun = globalNS.m_functions[0];
	CHECK(fun.m_name == "fun");
	CHECK(fun.m_arguments.size() == 0);
	TestUtil::compare(fun.m_returnType, IR::BaseType::Void);
}

TEST_CASE("Simple field", "[files]") {
	auto file = TestUtil::writeToFile(R"(
class MyClass {
	int i;
};
		)");
	auto globalNS = Parser::parseFile(file);
	SECTION("Parser finds the variable") {
		REQUIRE(globalNS.m_structs.size() == 1);
		auto myClass = globalNS.m_structs[0];
		REQUIRE(myClass.m_memberVariables.size() == 1);
		auto& [access, variable] = myClass.m_memberVariables.back();
		CHECK(variable.m_name == "i");
		REQUIRE_FALSE(variable.m_type.m_isConst);
		TestUtil::compare(variable.m_type, IR::BaseType::Int);
	}
}
