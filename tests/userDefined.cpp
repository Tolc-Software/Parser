#include "TestUtil/parse.hpp"
#include <IR/ir.hpp>
#include <catch2/catch.hpp>
#include <variant>

TEST_CASE("A simple user defined struct", "[userDefined]") {
	REQUIRE(1 == 1);
	auto globalNS = TestUtil::parseString(R"(
struct Simple {};

const Simple* f();
)");
	REQUIRE(globalNS.m_functions.size() == 1);
	auto& f = globalNS.m_functions[0];
	REQUIRE(f.m_name == "f");
	REQUIRE(f.m_returnType.m_representation == "const Simple *");
	auto returnType =
	    std::get_if<IR::Type::UserDefined>(&f.m_returnType.m_type);
	REQUIRE(returnType);
	REQUIRE(returnType->m_representation == "Simple");
}

TEST_CASE("A user defined class within a namespace", "[userDefined]") {
	REQUIRE(1 == 1);
	auto globalNS = TestUtil::parseString(R"(
namespace Nested {

struct Simple {};
}

Nested::Simple& f();
)");
	REQUIRE(globalNS.m_functions.size() == 1);
	auto& f = globalNS.m_functions[0];
	REQUIRE(f.m_name == "f");
	REQUIRE(f.m_returnType.m_representation == "Nested::Simple &");
	auto returnType =
	    std::get_if<IR::Type::UserDefined>(&f.m_returnType.m_type);
	REQUIRE(returnType);
	REQUIRE(returnType->m_representation == "Nested::Simple");
}
