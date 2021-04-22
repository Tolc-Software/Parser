#include "TestUtil/parse.hpp"
#include "TestUtil/types.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <variant>

TEST_CASE("std::stack of base type", "[stacks]") {
	auto code = R"(
#include <stack>
struct MyClass {
	std::stack<int> m_d;
};
)";
	CAPTURE(code);
	auto globalNS = TestUtil::parseString(code);
	REQUIRE(globalNS.m_structs.size() == 1);
	auto& myClass = globalNS.m_structs.back();
	REQUIRE(myClass.m_memberVariables.size() == 1);

	auto& [am, m_d] = myClass.m_memberVariables.back();
	REQUIRE(am == IR::AccessModifier::Public);
	REQUIRE(m_d.m_type.m_representation == "std::stack<int>");
	auto stackType = std::get_if<IR::Type::Container>(&m_d.m_type.m_type);
	REQUIRE(stackType != nullptr);

	REQUIRE(stackType->m_container == IR::ContainerType::Stack);
	// NOTE: Second arg is the allocator
	REQUIRE(stackType->m_containedTypes.size() == 2);
	auto& type = stackType->m_containedTypes.front();

	auto value = std::get_if<IR::Type::Value>(&type.m_type);
	REQUIRE(value != nullptr);
	REQUIRE(value->m_base == IR::BaseType::Int);
	REQUIRE(type.m_representation == "int");
}
