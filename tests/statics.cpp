#include "TestUtil/compare.hpp"
#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"
#include <IR/ir.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Static variable within function does not leak", "[statics]") {
	std::string code = R"(
void f() {
	static int i;
}
)";
	auto globalNS = TestUtil::parseString(code);
	REQUIRE(globalNS.m_variables.empty());
	auto& f = TestUtil::findFunction(globalNS, "f");

	REQUIRE(!f.m_isStatic);
}

TEST_CASE("Static global variable", "[statics]") {
	std::string code = R"(
static int i;
)";
	auto globalNS = TestUtil::parseString(code);
	auto& i = TestUtil::findVariable(globalNS, "i");

	REQUIRE(i.m_type.m_isStatic);
}

TEST_CASE("Static global function", "[statics]") {
	std::string code = R"(
static void f() {};
)";
	auto globalNS = TestUtil::parseString(code);
	auto& f = TestUtil::findFunction(globalNS, "f");

	REQUIRE(f.m_isStatic);
}

TEST_CASE("Static member function", "[statics]") {
	std::string code = R"(
struct MyClass {
	static void f() {};
};
)";
	auto globalNS = TestUtil::parseString(code);
	auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
	auto& f = TestUtil::findFunction(myClass, "f", IR::AccessModifier::Public);

	REQUIRE(f.m_isStatic);
}

TEST_CASE("Static member variable", "[statics]") {
	std::string code = R"(
struct MyClass
{
     static int i;
};
)";
	auto globalNS = TestUtil::parseString(code);
	auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
	auto& i = TestUtil::findMember(myClass, "i", IR::AccessModifier::Public);

	REQUIRE(i.m_type.m_isStatic);
}
