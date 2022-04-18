#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"
#include <catch2/catch.hpp>

TEST_CASE("Dependency between class types are predetermined", "[dependency]") {
	auto [globalNS, meta] = TestUtil::parseStringWithMeta(R"(

namespace MyLib {
	struct Data { };
}

MyLib::Data f();
)");
	REQUIRE(meta.m_definitionOrder.size() == 2);
	auto& myLib = TestUtil::findNamespace(globalNS, "MyLib");
	auto const& data = TestUtil::findStruct(myLib, "Data");

	auto const& f = TestUtil::findFunction(globalNS, "f");
	// f requires Data to be defined before
	REQUIRE(meta.m_definitionOrder[0] == data.m_id);
	REQUIRE(meta.m_definitionOrder[1] == f.m_id);
}

TEST_CASE("Dependency between enum types are predetermined", "[dependency]") {
	auto [globalNS, meta] = TestUtil::parseStringWithMeta(R"(

namespace MyLib {
enum class Data {
	d0,
	d1
};
}

MyLib::Data f();
)");
	REQUIRE(meta.m_definitionOrder.size() == 2);
	auto& myLib = TestUtil::findNamespace(globalNS, "MyLib");
	auto const& data = TestUtil::findEnum(myLib, "Data");

	auto const& f = TestUtil::findFunction(globalNS, "f");
	// f requires Data to be defined before
	REQUIRE(meta.m_definitionOrder[0] == data.m_id);
	REQUIRE(meta.m_definitionOrder[1] == f.m_id);
}

