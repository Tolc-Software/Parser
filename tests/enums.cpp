#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"
#include <algorithm>
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Scoped and unscoped enums", "[enums]") {
	auto code = R"(

enum class ScopedEnum {
	Thing0,
	Thing1,
	Thing2,
};

enum UnScopedEnum {
	Thing0,
	Thing1,
	Thing2,
};
)";
	CAPTURE(code);
	auto globalNS = TestUtil::parseString(code);
	REQUIRE(globalNS.m_enums.size() == 2);
	auto maybeScoped = TestUtil::findWithName("ScopedEnum", globalNS.m_enums);
	auto maybeUnscoped =
	    TestUtil::findWithName("UnScopedEnum", globalNS.m_enums);
	REQUIRE(maybeScoped.has_value());
	REQUIRE(maybeUnscoped.has_value());
	REQUIRE(maybeScoped->m_isScoped == true);
	REQUIRE(maybeUnscoped->m_isScoped == false);
	for (auto const& e : globalNS.m_enums) {
		REQUIRE(e.m_name == e.m_representation);
		REQUIRE(e.m_values.size() == 3);
		for (std::string value : {"Thing0", "Thing1", "Thing2"}) {
			REQUIRE(std::find(e.m_values.begin(), e.m_values.end(), value) !=
			        e.m_values.end());
		}
	}
}

TEST_CASE("Enum in structs", "[enums]") {
	auto code = R"(

class C {
	enum class MyEnum { Value };
};

struct S {
	enum class MyEnum { Value };
};

)";
	CAPTURE(code);
	auto globalNS = TestUtil::parseString(code);
	REQUIRE(globalNS.m_structs.size() == 2);
	auto maybeC = TestUtil::findWithName("C", globalNS.m_structs);
	auto maybeS = TestUtil::findWithName("S", globalNS.m_structs);
	REQUIRE(maybeS.has_value());
	REQUIRE(maybeC.has_value());
	for (auto const& s : globalNS.m_structs) {
		CAPTURE(s.m_name);
		REQUIRE(s.m_enums.size() == 1);
		auto& [am, e] = s.m_enums.back();
		CAPTURE(e.m_name);
		CAPTURE(e.m_representation);
		if (s.m_name == "S") {
			REQUIRE(am == IR::AccessModifier::Public);
		} else {
			REQUIRE(am == IR::AccessModifier::Private);
		}
		REQUIRE(e.m_isScoped == true);
		REQUIRE(e.m_representation ==
		        fmt::format("{className}::{enumName}",
		                    fmt::arg("className", s.m_name),
		                    fmt::arg("enumName", e.m_name)));

		REQUIRE(e.m_values.size() == 1);
		REQUIRE(e.m_values.back() == "Value");
	}
}
