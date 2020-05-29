#include "TestUtil/parse.hpp"
#include "TestUtil/types.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <variant>

TEST_CASE("std::array of base type", "[arrays]") {
	// TODO: There is a bug with std::string within containers where the representation get garbled
	for (auto baseType :
	     TestUtil::getBaseTypes(/* excluding */ {"std::string", "void"})) {
		auto code = R"(
#include <array>

struct MyClass {
)" +
		            fmt::format(R"(
	std::array<{baseType}, 5> m_v;)",
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
		        fmt::format("std::array<{baseType}, 5>",
		                    fmt::arg("baseType", baseType)));
		auto arrayType = std::get_if<IR::Type::Container>(&m_v.m_type.m_type);
		REQUIRE(arrayType != nullptr);

		REQUIRE(arrayType->m_container == IR::ContainerType::Array);
		// {baseType} - NOTE: Second arg is always size_t but as you cannot change its type, it is not recorded
		REQUIRE(arrayType->m_containedTypes.size() == 1);
		auto& type = arrayType->m_containedTypes.back();

		if (auto value = std::get_if<IR::Type::Value>(&type.m_type)) {
			auto base = TestUtil::getIRFromString(baseType);
			REQUIRE(base.has_value());
			REQUIRE(value->m_base == base.value());
			REQUIRE(type.m_isConst == false);
			REQUIRE(type.m_isReference == false);
			REQUIRE(type.m_representation == baseType);
		} else {
			INFO(
			    "The type is not a base type. Something must have gone wrong.");
			REQUIRE(false);
		}
	}
}