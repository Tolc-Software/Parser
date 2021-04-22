#include "TestUtil/parse.hpp"
#include "TestUtil/types.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <variant>

TEST_CASE("std::unordered_multimap of base type", "[unordered_multimaps]") {
	auto code = R"(
#include <unordered_map>
struct MyClass {
	std::unordered_multimap<int, int> m_d;
};
)";
	CAPTURE(code);
	auto globalNS = TestUtil::parseString(code);
	REQUIRE(globalNS.m_structs.size() == 1);
	auto& myClass = globalNS.m_structs.back();
	REQUIRE(myClass.m_memberVariables.size() == 1);

	auto& [am, m_d] = myClass.m_memberVariables.back();
	REQUIRE(am == IR::AccessModifier::Public);
	REQUIRE(m_d.m_type.m_representation == "std::unordered_multimap<int, int>");
	auto multimapType = std::get_if<IR::Type::Container>(&m_d.m_type.m_type);
	REQUIRE(multimapType != nullptr);

	REQUIRE(multimapType->m_container == IR::ContainerType::UnorderedMultiMap);
	// NOTE: Third arg is the hash, fourth is the comparator, fifth is the allocator
	REQUIRE(multimapType->m_containedTypes.size() == 5);
	auto& type = multimapType->m_containedTypes.front();

	auto value = std::get_if<IR::Type::Value>(&type.m_type);
	REQUIRE(value != nullptr);
	REQUIRE(value->m_base == IR::BaseType::Int);
	REQUIRE(type.m_representation == "int");
}
