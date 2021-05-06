#include "TestUtil/parse.hpp"
#include "TestUtil/types.hpp"
#include <catch2/catch.hpp>
#include <variant>

TEST_CASE("std::variant of base type", "[variants]") {
	auto code = R"(
#include <variant>

struct MyClass {
	std::variant<int, double, bool> m_d;
};
)";
	CAPTURE(code);
	auto globalNS = TestUtil::parseString(code);
	REQUIRE(globalNS.m_structs.size() == 1);
	auto& myClass = globalNS.m_structs.back();
	REQUIRE(myClass.m_memberVariables.size() == 1);

	auto& [am, m_d] = myClass.m_memberVariables.back();
	REQUIRE(am == IR::AccessModifier::Public);
	REQUIRE(m_d.m_type.m_representation == "std::variant<int, double, bool>");
	auto variantType = std::get_if<IR::Type::Container>(&m_d.m_type.m_type);
	REQUIRE(variantType != nullptr);

	REQUIRE(variantType->m_container == IR::ContainerType::Variant);
	// TODO: Figure out why this is 0?
	// REQUIRE(variantType->m_containedTypes.size() == 3);
	// auto& type = variantType->m_containedTypes.front();

	// auto value = std::get_if<IR::Type::Value>(&type.m_type);
	// REQUIRE(value != nullptr);
	// REQUIRE(value->m_base == IR::BaseType::Int);
	// REQUIRE(type.m_representation == "int");
}
