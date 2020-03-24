#include "TestUtil/files.h"
#include "Parser/Parse.h"
#include <catch2/catch.hpp>

TEST_CASE("Can parse a file", "[files]") {
	auto file = TestUtil::writeToFile(R"(
namespace MyNamespace {}
)");
	auto globalNS = Parser::parseFile(file);
	REQUIRE(globalNS.m_namespaces.size() == 1);
	auto& testNs = globalNS.m_namespaces[0];

	REQUIRE(testNs.m_name == "MyNamespace");
}
