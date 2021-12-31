#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"
#include "TestUtil/types.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <variant>

TEST_CASE("std::forward_list of base type", "[forward_lists]") {
	auto code = R"(
#include <forward_list>
struct MyClass {
	std::forward_list<int> m_d;
};
)";
	CAPTURE(code);
	auto globalNS = TestUtil::parseString(code);
	auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
	auto& m_d =
	    TestUtil::findMember(myClass, "m_d", IR::AccessModifier::Public);

	REQUIRE(m_d.m_type.m_representation == "std::forward_list<int>");
	auto listType = std::get_if<IR::Type::Container>(&m_d.m_type.m_type);
	REQUIRE(listType != nullptr);

	REQUIRE(listType->m_container == IR::ContainerType::ForwardList);
	// NOTE: Second arg is the allocator
	REQUIRE(listType->m_containedTypes.size() == 2);
	auto& type = listType->m_containedTypes.front();

	auto value = std::get_if<IR::Type::Value>(&type.m_type);
	REQUIRE(value != nullptr);
	REQUIRE(value->m_base == IR::BaseType::Int);
	REQUIRE(type.m_representation == "int");
}
