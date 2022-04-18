#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"
#include <catch2/catch.hpp>

TEST_CASE("Two functions with struct dependency", "[dependency]") {
	auto [globalNS, meta] = TestUtil::parseStringWithMeta(R"(
struct Data {};

Data f();
Data g();
)");
	REQUIRE(meta.m_definitionOrder.size() == 3);
	auto const& data = TestUtil::findStruct(globalNS, "Data");

	// f,g requires Data to be defined before
	// Don't care which order they come in
	REQUIRE(meta.m_definitionOrder[0] == data.m_id);
}

TEST_CASE("Function with enum dependency, forward declaration",
          "[dependency]") {
	auto [globalNS, meta] = TestUtil::parseStringWithMeta(R"(
enum Data;
Data* f();

enum Data { D0 };
)");
	REQUIRE(meta.m_definitionOrder.size() == 2);
	auto const& data = TestUtil::findEnum(globalNS, "Data");

	auto const& f = TestUtil::findFunction(globalNS, "f");
	// f requires Data to be defined before
	REQUIRE(meta.m_definitionOrder[0] == data.m_id);
	REQUIRE(meta.m_definitionOrder[1] == f.m_id);
}

TEST_CASE("Function with struct dependency, forward declaration",
          "[dependency]") {
	auto [globalNS, meta] = TestUtil::parseStringWithMeta(R"(
struct Data;
Data* f();

struct Data {};
)");
	REQUIRE(meta.m_definitionOrder.size() == 2);
	auto const& data = TestUtil::findStruct(globalNS, "Data");

	auto const& f = TestUtil::findFunction(globalNS, "f");
	// f requires Data to be defined before
	REQUIRE(meta.m_definitionOrder[0] == data.m_id);
	REQUIRE(meta.m_definitionOrder[1] == f.m_id);
}

TEST_CASE("Class with nested enum", "[dependency]") {
	auto [globalNS, meta] = TestUtil::parseStringWithMeta(R"(

namespace MyLib {
	struct Data {
		enum class Inner { I0, I1 };
	};
}

MyLib::Data::Inner f();
)");
	// {MyLib::Data, MyLib::Data::Inner, f}
	REQUIRE(meta.m_definitionOrder.size() == 3);
	auto& myLib = TestUtil::findNamespace(globalNS, "MyLib");
	auto const& data = TestUtil::findStruct(myLib, "Data");
	auto const& inner =
	    TestUtil::findEnum(data, "Inner", TestUtil::AccessModifier::Public);
	auto const& f = TestUtil::findFunction(globalNS, "f");

	// f requires Inner to be defined
	// Inner requires Data to be defined before
	// => Data, Inner, f
	REQUIRE(meta.m_definitionOrder[0] == data.m_id);
	REQUIRE(meta.m_definitionOrder[1] == inner.m_id);
	REQUIRE(meta.m_definitionOrder[2] == f.m_id);
}

TEST_CASE("Function with struct dependency", "[dependency]") {
	auto [globalNS, meta] = TestUtil::parseStringWithMeta(R"(

namespace MyLib {
	struct Data {};
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
	enum class Data { d0, d1 };
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
