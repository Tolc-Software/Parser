#include "Helpers/Type/matchers.hpp"
#include <catch2/catch.hpp>

TEST_CASE("User defined strings are parsed correctly", "[matchers]") {
	for (auto const& className : {"MyClass", "struct", "structeD"}) {
		std::string userDefined = std::string("struct ") + className;
		CAPTURE(userDefined);
		auto ud = Helpers::Type::getUserDefinedType(userDefined);
		REQUIRE(ud.has_value());
		REQUIRE(ud.value().m_representation == className);
	}
}

TEST_CASE("Non user defined types are rejected", "[matchers]") {
	for (auto const& type : {"int", "double", "whateverStruct"}) {
		CAPTURE(type);
		auto ud = Helpers::Type::getUserDefinedType(type);
		REQUIRE(!ud.has_value());
	}
}
