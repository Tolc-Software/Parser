#include "TestUtil/parse.hpp"
#include "TestUtil/types.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <variant>

TEST_CASE("std::map of base type", "[maps]") {
	auto code = R"(
#include <map>

struct MyClass {
	std::map<int, int> m_v;
};

)";
	CAPTURE(code);
	auto globalNS = TestUtil::parseString(code);
	REQUIRE(globalNS.m_structs.size() == 1);
	auto& myClass = globalNS.m_structs.back();
	REQUIRE(myClass.m_memberVariables.size() == 1);
	auto& [am, m_v] = myClass.m_memberVariables.back();
	REQUIRE(am == IR::AccessModifier::Public);
	REQUIRE(m_v.m_type.m_representation == "std::map<int, int>");
	auto mapType = std::get_if<IR::Type::Container>(&m_v.m_type.m_type);
	REQUIRE(mapType != nullptr);

	REQUIRE(mapType->m_container == IR::ContainerType::Map);
	// class std::map<int, int, struct std::less<int>, class std::allocator<struct std::pair<const int, int> > >
	REQUIRE(mapType->m_containedTypes.size() == 4);

	for (auto const& type : mapType->m_containedTypes) {
		if (auto value = std::get_if<IR::Type::Value>(&type.m_type)) {
			REQUIRE(value->m_base == IR::BaseType::Int);
			REQUIRE(type.m_isConst == false);
			REQUIRE(type.m_isReference == false);
			REQUIRE(type.m_representation == "int");
		} else if (auto container =
		               std::get_if<IR::Type::Container>(&type.m_type)) {
			REQUIRE(container->m_containedTypes.size() == 1);
			auto containerType = container->m_container;
			REQUIRE((containerType == IR::ContainerType::Less ||
			         containerType == IR::ContainerType::Allocator));
			if (containerType == IR::ContainerType::Allocator) {
				auto& pairType = container->m_containedTypes.back();
				auto pair = std::get_if<IR::Type::Container>(&pairType.m_type);
				REQUIRE(pair != nullptr);
				REQUIRE(pair->m_containedTypes.size() == 2);
			}
		} else {
			INFO(
			    "The map does not contain expected templated values. Something must have gone wrong.");
			REQUIRE(false);
		}
	}
}
