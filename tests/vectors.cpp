#include "TestUtil/parse.hpp"
#include "TestUtil/types.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <variant>

TEST_CASE("Deeply nested classes", "[vector]") {
	for (auto baseType :
	     TestUtil::getBaseTypes(/* excluding */ {"std::string", "void"})) {
		auto code = R"(
#include <vector>

struct MyClass {
)" +
		            fmt::format(R"(
	std::vector<{baseType}> m_v;)",
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
	            fmt::format("std::vector<{baseType}>",
	                        fmt::arg("baseType", baseType)));
	    auto vectorType = std::get_if<IR::Type::Container>(&m_v.m_type.m_type);
		REQUIRE(vectorType != nullptr);

		REQUIRE(vectorType->m_container == IR::ContainerType::Vector);
		// {{baseType}, std::allocator<{baseType}>}
		REQUIRE(vectorType->m_containedTypes.size() == 2);
		for (auto const& type : vectorType->m_containedTypes) {
			if (auto value = std::get_if<IR::Type::Value>(&type.m_type)) {
				auto base = TestUtil::getIRFromString(baseType);
				REQUIRE(base.has_value());
				REQUIRE(value->m_base == base.value());
				REQUIRE(type.m_isConst == false);
				REQUIRE(type.m_isReference == false);
				REQUIRE(type.m_representation == baseType);
			} else if (auto allocator =
			               std::get_if<IR::Type::Container>(&type.m_type)) {
				REQUIRE(allocator->m_container == IR::ContainerType::Allocator);
				REQUIRE(type.m_isConst == false);
				REQUIRE(type.m_isReference == false);
			    REQUIRE(type.m_representation ==
			            fmt::format("std::allocator<{baseType}>",
			                        fmt::arg("baseType", baseType)));

			    REQUIRE(allocator->m_containedTypes.size() == 1);
				auto& allocInt = allocator->m_containedTypes.back();
				REQUIRE(allocInt.m_isConst == false);
				REQUIRE(allocInt.m_isReference == false);
				REQUIRE(allocInt.m_representation == baseType);

				auto valueInt = std::get_if<IR::Type::Value>(&allocInt.m_type);
				REQUIRE(valueInt != nullptr);
				auto base = TestUtil::getIRFromString(baseType);
				REQUIRE(base.has_value());
				REQUIRE(valueInt->m_base == base.value());

			} else {
				INFO(
				    "The type is neither a int or allocator. Something must have gone wrong.");
				REQUIRE(false);
			}
		}
	}
}
