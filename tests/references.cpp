#include "TestUtil/parse.hpp"
#include <catch2/catch.hpp>

TEST_CASE("Class with reference variables", "[references]") {
	auto code = R"(
#include <string>

struct MyClass {
	int i;
	double& d;
	const std::string& s;
};

)";
	CAPTURE(code);
	auto globalNS = TestUtil::parseString(code);
	REQUIRE(globalNS.m_structs.size() == 1);
	auto& myClass = globalNS.m_structs.back();
	REQUIRE(myClass.m_memberVariables.size() == 3);

	for (auto const& [am, var] : myClass.m_memberVariables) {
		if (var.m_name == "i") {
			auto i = var.m_type;
			REQUIRE(!i.m_isConst);
			REQUIRE(!i.m_isReference);
		} else if (var.m_name == "d") {
			auto d = var.m_type;
			REQUIRE(!d.m_isConst);
			REQUIRE(d.m_isReference);
		} else if (var.m_name == "s") {
			auto s = var.m_type;
			REQUIRE(s.m_isConst);
			REQUIRE(s.m_isReference);
		}
	}
}

TEST_CASE("Function with reference parameter", "[references]") {
	auto code = R"(
#include <string>

void f(std::string const& s);

)";
	CAPTURE(code);
	auto globalNS = TestUtil::parseString(code);
	REQUIRE(globalNS.m_functions.size() == 1);
	auto& f = globalNS.m_functions.back();
	REQUIRE(f.m_arguments.size() == 1);
	auto& s = f.m_arguments.back();
	REQUIRE(s.m_type.m_isReference);
	REQUIRE(s.m_type.m_isConst);
	REQUIRE(s.m_type.m_numPointers == 0);
}
