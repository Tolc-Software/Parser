#include "TestUtil/parse.hpp"
#include "TestUtil/types.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <variant>

TEST_CASE("std::set of base type", "[sets]") {
	// TODO: There is a bug with std::string within containers where the representation get garbled
	for (auto baseType :
	     TestUtil::getBaseTypes(/* excluding */ {"std::string", "void"})) {
		auto code = R"(
#include <set>

struct MyClass {
)" +
		            fmt::format(R"(
	std::set<{baseType}> m_v;)",
		                        fmt::arg("baseType", baseType)) +
		            R"(
};

)";
		CAPTURE(code);
		auto globalNS = TestUtil::parseString(code);
		REQUIRE(globalNS.m_structs.size() == 1);
		auto& myClass = globalNS.m_structs.back();
		REQUIRE(myClass.m_memberVariables.size() == 1);
		auto& [am, m_v] = myClass.m_memberVariables.back();
		REQUIRE(am == IR::AccessModifier::Public);
		REQUIRE(m_v.m_type.m_representation ==
		        fmt::format("std::set<{baseType}>",
		                    fmt::arg("baseType", baseType)));
		auto arrayType = std::get_if<IR::Type::Container>(&m_v.m_type.m_type);
		REQUIRE(arrayType != nullptr);

		REQUIRE(arrayType->m_container == IR::ContainerType::Set);
		// std::set<{baseType}, struct std::less<{baseType}>, class std::allocator<{baseType}> >
		REQUIRE(arrayType->m_containedTypes.size() == 3);

		for (auto const& type : arrayType->m_containedTypes) {
			if (auto value = std::get_if<IR::Type::Value>(&type.m_type)) {
				auto base = TestUtil::getIRFromString(baseType);
				REQUIRE(base.has_value());
				REQUIRE(value->m_base == base.value());
				REQUIRE(type.m_isConst == false);
				REQUIRE(type.m_isReference == false);
				REQUIRE(type.m_representation == baseType);
			} else if (auto container =
			               std::get_if<IR::Type::Container>(&type.m_type)) {
				REQUIRE(container->m_containedTypes.size() == 1);
				auto containerType = container->m_container;
				REQUIRE((containerType == IR::ContainerType::Less ||
				         containerType == IR::ContainerType::Allocator));
			} else {
				INFO(
				    "The set does not contain expected templated values. Something must have gone wrong.");
				REQUIRE(false);
			}
		}
	}
}