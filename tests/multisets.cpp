#include "TestUtil/parse.hpp"
#include "TestUtil/types.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <variant>

TEST_CASE("std::multiset of base type", "[multisets]") {
	auto code = R"(
#include <set>
struct MyClass {
	std::multiset<int> m_d;
};
)";
	CAPTURE(code);
	auto globalNS = TestUtil::parseString(code);
	REQUIRE(globalNS.m_structs.size() == 1);
	auto& myClass = globalNS.m_structs.back();
	REQUIRE(myClass.m_memberVariables.size() == 1);

	auto& [am, m_d] = myClass.m_memberVariables.back();
	REQUIRE(am == IR::AccessModifier::Public);
	REQUIRE(m_d.m_type.m_representation == "std::multiset<int>");
	auto multisetType = std::get_if<IR::Type::Container>(&m_d.m_type.m_type);
	REQUIRE(multisetType != nullptr);

	REQUIRE(multisetType->m_container == IR::ContainerType::MultiSet);
	// NOTE: Second arg is the comparator, third is the allocator
	REQUIRE(multisetType->m_containedTypes.size() == 3);
	auto& type = multisetType->m_containedTypes.front();

	auto value = std::get_if<IR::Type::Value>(&type.m_type);
	REQUIRE(value != nullptr);
	REQUIRE(value->m_base == IR::BaseType::Int);
	REQUIRE(type.m_representation == "int");
}
