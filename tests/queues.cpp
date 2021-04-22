#include "TestUtil/parse.hpp"
#include "TestUtil/types.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <variant>

TEST_CASE("std::queue of base type", "[queues]") {
	auto code = R"(
#include <queue>
struct MyClass {
	std::queue<int> m_d;
};
)";
	CAPTURE(code);
	auto globalNS = TestUtil::parseString(code);
	REQUIRE(globalNS.m_structs.size() == 1);
	auto& myClass = globalNS.m_structs.back();
	REQUIRE(myClass.m_memberVariables.size() == 1);

	auto& [am, m_d] = myClass.m_memberVariables.back();
	REQUIRE(am == IR::AccessModifier::Public);
	REQUIRE(m_d.m_type.m_representation == "std::queue<int>");
	auto queueType = std::get_if<IR::Type::Container>(&m_d.m_type.m_type);
	REQUIRE(queueType != nullptr);

	REQUIRE(queueType->m_container == IR::ContainerType::Queue);
	// NOTE: Second arg is the allocator
	REQUIRE(queueType->m_containedTypes.size() == 2);
	auto& type = queueType->m_containedTypes.front();

	auto value = std::get_if<IR::Type::Value>(&type.m_type);
	REQUIRE(value != nullptr);
	REQUIRE(value->m_base == IR::BaseType::Int);
	REQUIRE(type.m_representation == "int");
}
