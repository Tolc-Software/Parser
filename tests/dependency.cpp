#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"
#include <catch2/catch.hpp>

TEST_CASE("One function depends on two structs", "[dependency]") {
	auto [globalNS, meta] = TestUtil::parseStringWithMeta(R"(
struct Data0 {};
struct Data1 {};

struct Consumer {
	Consumer(Data0 d0, Data1 d1);
};
)");
	REQUIRE(meta.m_definitionOrder.size() == 4);
	auto const& data0 = TestUtil::findStruct(globalNS, "Data0");
	auto const& data1 = TestUtil::findStruct(globalNS, "Data1");
	auto const& consumer = TestUtil::findStruct(globalNS, "Consumer");
	REQUIRE(consumer.m_public.m_constructors.size() == 1);
	auto const& consumerConstructor = consumer.m_public.m_constructors.back();

	// Data0 comes first in the file
	// Consumer requires both Data0 and Data1 to be defined
	REQUIRE(meta.m_definitionOrder[0] == data0.m_id);
	REQUIRE(meta.m_definitionOrder[1] == data1.m_id);
	REQUIRE(meta.m_definitionOrder[2] == consumer.m_id);
	REQUIRE(meta.m_definitionOrder[3] == consumerConstructor.m_id);
}

TEST_CASE("Linear dependency", "[dependency]") {
	auto [globalNS, meta] = TestUtil::parseStringWithMeta(R"(
struct Data {};

struct Consumer {
	Consumer(Data d);
};

void f(Consumer);
)");
	REQUIRE(meta.m_definitionOrder.size() == 4);
	auto const& data = TestUtil::findStruct(globalNS, "Data");
	auto const& consumer = TestUtil::findStruct(globalNS, "Consumer");
	REQUIRE(consumer.m_public.m_constructors.size() == 1);
	auto const& consumerConstructor = consumer.m_public.m_constructors.back();
	auto const& f = TestUtil::findFunction(globalNS, "f");

	// f requires Consumer which requires Data
	REQUIRE(meta.m_definitionOrder[0] == data.m_id);
	REQUIRE(meta.m_definitionOrder[1] == consumer.m_id);
	REQUIRE(meta.m_definitionOrder[2] == consumerConstructor.m_id);
	REQUIRE(meta.m_definitionOrder[3] == f.m_id);
}

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

TEST_CASE("Function requires namespaced struct", "[dependency]") {
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

TEST_CASE("Function requires namespaced enum", "[dependency]") {
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
