#include "TestUtil/parse.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <string>

TEST_CASE(
    "Simple data class with one container of strings - should have an implicitly created default constructor if no other constructor is available",
    "[dataStructs]") {
	for (bool hasUserDefinedConstructor : {true, false}) {
		std::string constructor =
		    hasUserDefinedConstructor ? "MyClass(std::string);" : "";
		for (std::string container :
		     {"vector", "list", "set", "unordered_set"}) {
			for (std::string accessor : {"public", "private", "protected"}) {
				auto code = fmt::format(R"(
#include <string>
#include <{container}>

struct MyClass {{
	{constructor}
{accessor}:
	std::{container}<std::string> s;
}};
)",
				                        fmt::arg("container", container),
				                        fmt::arg("constructor", constructor),
				                        fmt::arg("accessor", accessor));
				CAPTURE(code);
				auto globalNS = TestUtil::parseString(code);
				REQUIRE(globalNS.m_structs.size() == 1);
				auto& myStruct = globalNS.m_structs.back();
				REQUIRE(myStruct.m_hasImplicitDefaultConstructor ==
				        !hasUserDefinedConstructor);
			}
		}
	}
}
