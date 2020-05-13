#include "TestUtil/parse.hpp"
#include <catch2/catch.hpp>
#include <variant>

TEST_CASE("Deeply nested classes", "[containers]") {
	auto code = R"(
#include <vector>

struct MyClass {
	std::vector<int> m_v;
};

)";
	CAPTURE(code);
	auto globalNS = TestUtil::parseString(code);
	REQUIRE(globalNS.m_structs.size() == 1);
	auto& myClass = globalNS.m_structs.back();
	REQUIRE(myClass.m_memberVariables.size() == 1);
	auto& [am, m_v] = myClass.m_memberVariables.back();
	REQUIRE(am == IR::AccessModifier::Public);
	REQUIRE(m_v.m_type.m_representation == "std::vector<int>");
	auto vectorType = std::get_if<IR::Type::Container>(&m_v.m_type.m_type);
	REQUIRE(vectorType != nullptr);

	REQUIRE(vectorType->m_container == IR::ContainerType::Vector);
	// {int, std::allocator<int>}
	REQUIRE(vectorType->m_containedTypes.size() == 2);
}
