#include "TestUtil/parse.hpp"
#include <catch2/catch.hpp>
#include <string>

TEST_CASE(
    "Simple data class with one container of strings - should be default constructible",
    "[dataStructs]") {
	for (std::string container : {"vector", "list", "set", "unordered_set"}) {
		for (std::string accessor : {"public", "private", "protected"}) {
			auto code = R"(
#include <)" + container +
			            R"(>
#include <string>

struct MyClass {
)" + accessor + R"(:
	std::)" + container +
			            R"(<std::string> s;
};
)";
			CAPTURE(code);
			auto globalNS = TestUtil::parseString(code);
			REQUIRE(globalNS.m_structs.size() == 1);
			auto& myStruct = globalNS.m_structs.back();
			REQUIRE(myStruct.m_hasDefaultConstructor);
		}
	}
}
