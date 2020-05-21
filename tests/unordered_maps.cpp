#include "TestUtil/parse.hpp"
#include "TestUtil/types.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <variant>

TEST_CASE("std::unordered_map of base types", "[unordered_map]") {
	for (auto baseType :
	     TestUtil::getBaseTypes(/* excluding */ {"std::string", "void"})) {
		auto code = R"(
#include <unordered_map>

struct MyClass {
)" +
		            fmt::format(R"(
	std::unordered_map<{baseType}, {baseType}> m_v;)",
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
		        fmt::format("std::unordered_map<{baseType}, {baseType}>",
		                    fmt::arg("baseType", baseType)));
		// auto mapType = std::get_if<IR::Type::Container>(&m_v.m_type.m_type);
		// REQUIRE(mapType != nullptr);

		// REQUIRE(mapType->m_container == IR::ContainerType::Unordered_map);
		// // {{baseType}, std::allocator<{baseType}>}
		// REQUIRE(mapType->m_containedTypes.size() == 2);
		// for (auto const& type : mapType->m_containedTypes) {
		// 	if (auto value = std::get_if<IR::Type::Value>(&type.m_type)) {
		// 		auto base = TestUtil::getIRFromString(baseType);
		// 		REQUIRE(base.has_value());
		// 		REQUIRE(value->m_base == base.value());
		// 		REQUIRE(type.m_isConst == false);
		// 		REQUIRE(type.m_isReference == false);
		// 		REQUIRE(type.m_representation == baseType);
		// 	} else if (auto allocator =
		// 	               std::get_if<IR::Type::Container>(&type.m_type)) {
		// 		REQUIRE(allocator->m_container == IR::ContainerType::Allocator);
		// 		REQUIRE(type.m_isConst == false);
		// 		REQUIRE(type.m_isReference == false);
		// 	    REQUIRE(type.m_representation ==
		// 	            fmt::format("std::allocator<{baseType}>",
		// 	                        fmt::arg("baseType", baseType)));

		// 	    REQUIRE(allocator->m_containedTypes.size() == 1);
		// 		auto& allocInt = allocator->m_containedTypes.back();
		// 		REQUIRE(allocInt.m_isConst == false);
		// 		REQUIRE(allocInt.m_isReference == false);
		// 		REQUIRE(allocInt.m_representation == baseType);

		// 		auto valueInt = std::get_if<IR::Type::Value>(&allocInt.m_type);
		// 		REQUIRE(valueInt != nullptr);
		// 		auto base = TestUtil::getIRFromString(baseType);
		// 		REQUIRE(base.has_value());
		// 		REQUIRE(valueInt->m_base == base.value());

		// 	} else {
		// 		INFO(
		// 		    "The type is neither a int or allocator. Something must have gone wrong.");
		// 		REQUIRE(false);
		// 	}
		// }
	}
}
