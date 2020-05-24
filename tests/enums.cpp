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

TEST_CASE("enum as a type in a function", "[enums]") {
	auto code = R"(

enum class E {
	V0,
};

namespace MyLib {
	enum class E2 {
		V2,
	};
}

E f(MyLib::E2 e);
)";
	CAPTURE(code);
	auto globalNS = TestUtil::parseString(code);
	REQUIRE(globalNS.m_functions.size() == 1);
	auto& f = globalNS.m_functions.back();
	REQUIRE(f.m_returnType.m_isConst == false);
	REQUIRE(f.m_returnType.m_isReference == false);
	REQUIRE(f.m_returnType.m_representation == "E");

	REQUIRE(f.m_arguments.size() == 1);
	REQUIRE(f.m_arguments.size() == 1);
	auto& e = f.m_arguments.back();
	REQUIRE(e.m_name == "e");
	REQUIRE(e.m_type.m_isConst == false);
	REQUIRE(e.m_type.m_isReference == false);
	REQUIRE(e.m_type.m_representation == "MyLib::E2");
}

TEST_CASE("enum as a type in a class", "[enums]") {
	auto code = R"(

enum class E {
	V0,
};

namespace MyLib {
	enum class E2 {
		V2,
	};
}

class C {
public:
	E m_e;
private:
	MyLib::E2 m_e2;
};
)";
	CAPTURE(code);
	auto globalNS = TestUtil::parseString(code);
	REQUIRE(globalNS.m_structs.size() == 1);
	auto& C = globalNS.m_structs.back();
	REQUIRE(C.m_memberVariables.size() == 2);
	for (auto const& [am, e] : C.m_memberVariables) {
		CAPTURE(e.m_name);
		auto enumValue = std::get_if<IR::Type::EnumValue>(&e.m_type.m_type);
		REQUIRE(enumValue != nullptr);
		if (e.m_name == "m_e") {
			REQUIRE(am == IR::AccessModifier::Public);
			REQUIRE(enumValue->m_representation == "E");
		} else if (e.m_name == "m_e2") {
			REQUIRE(am == IR::AccessModifier::Private);
			REQUIRE(enumValue->m_representation == "MyLib::E2");
		} else {
			REQUIRE(false);
		}
		REQUIRE(e.m_type.m_isConst == false);
		REQUIRE(e.m_type.m_isReference == false);
	}
}

