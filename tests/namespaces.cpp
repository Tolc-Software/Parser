#include "Parser/Parse.h"
#include <algorithm>
#include <catch2/catch.hpp>
#include <string_view>

namespace {
void checkEmpty(IR::Namespace& ns) {
	CAPTURE(ns.m_name);

	CHECK(ns.m_functions.empty());
	CHECK(ns.m_structs.empty());
}
}    // namespace

TEST_CASE("Single namespace", "[namespaces]") {
	auto globalNS = Parser::parseString("namespace Test {}");
	SECTION("Parser finds an empty namespace named Test") {
		REQUIRE(globalNS.m_namespaces.size() == 1);
		auto& testNs = globalNS.m_namespaces[0];

		checkEmpty(testNs);

		REQUIRE(testNs.m_name == "Test");
	}
}

TEST_CASE("Two namespaces", "[namespaces]") {
	auto globalNS = Parser::parseString(R"(
namespace Test0 {}
namespace Test1 {}
		)");
	SECTION("Parser finds two empty namespaces") {
		auto& namespaces = globalNS.m_namespaces;
		REQUIRE(namespaces.size() == 2);
		for (auto& ns : namespaces) {
			checkEmpty(ns);
		}

		SECTION("Named correctly") {
			for (auto name : {"Test0", "Test1"}) {
				CAPTURE(name);
				REQUIRE(std::any_of(
				    namespaces.begin(),
				    namespaces.end(),
				    [name](auto const& ns) { return ns.m_name == name; }));
			}
		}
	}
}

TEST_CASE("Two nested namespaces", "[namespaces]") {
	auto globalNS = Parser::parseString(R"(
namespace ParentNamespace {
	namespace ChildNamespace {}
}
		)");
	SECTION("Parser finds the empty root namespace") {
		auto& namespaces = globalNS.m_namespaces;
		REQUIRE(namespaces.size() == 1);
		auto& ns = namespaces.back();
		checkEmpty(ns);

		// It has one child
		REQUIRE(ns.m_namespaces.size() == 1);
		auto& child = ns.m_namespaces.back();
		checkEmpty(ns);

		SECTION("Named correctly") {
			CHECK(ns.m_name == "ParentNamespace");
			CHECK(child.m_name == "ChildNamespace");
		}

		SECTION("Nested correctly") {
			CHECK(child.m_parent == "ParentNamespace");
		}
	}
}

TEST_CASE("Two namespaces with the same name and level are the same",
          "[namespaces]") {
	auto globalNS = Parser::parseString(R"(
namespace Name {
}
namespace Name {
}
		)");
	SECTION("Parser correctly merges the namespace") {
		auto& namespaces = globalNS.m_namespaces;
		REQUIRE(namespaces.size() == 1);
		auto& ns = namespaces.back();
		checkEmpty(ns);

		SECTION("Named correctly") {
			CHECK(ns.m_name == "Name");
		}
	}
}

TEST_CASE("Three nested namespaces", "[namespaces]") {
	auto globalNS = Parser::parseString(R"(
namespace ParentNamespace {
	namespace ChildNamespace {
		namespace GrandchildNamespace {}
	}
}
		)");
	SECTION("Parser finds the empty root namespace") {
		auto& namespaces = globalNS.m_namespaces;
		REQUIRE(namespaces.size() == 1);
		auto& root = namespaces.back();
		checkEmpty(root);

		REQUIRE(root.m_namespaces.size() == 1);
		auto& child = root.m_namespaces.back();
		checkEmpty(child);

		REQUIRE(child.m_namespaces.size() == 1);
		auto& grandchild = child.m_namespaces.back();
		checkEmpty(grandchild);

		SECTION("Named correctly") {
			CHECK(root.m_name == "ParentNamespace");
			CHECK(child.m_name == "ChildNamespace");
			CHECK(grandchild.m_name == "GrandchildNamespace");
		}

		SECTION("Nested correctly") {
			CHECK(child.m_parent == "ParentNamespace");
			CHECK(grandchild.m_parent == "ChildNamespace");
		}
	}
}

TEST_CASE("Same name but different qualified name", "[namespaces]") {
	auto globalNS = Parser::parseString(R"(
namespace A {
	namespace B {}
}
namespace B {}
		)");
	SECTION("Parser finds the two empty root namespaces") {
		auto& namespaces = globalNS.m_namespaces;
		REQUIRE(namespaces.size() == 2);
		checkEmpty(namespaces[0]);
		checkEmpty(namespaces[1]);
		auto a = std::find_if(namespaces.begin(),
		                      namespaces.end(),
		                      [](auto const& ns) { return ns.m_name == "A"; });
		REQUIRE(a != namespaces.end());
		auto b = std::find_if(namespaces.begin(),
		                      namespaces.end(),
		                      [](auto const& ns) { return ns.m_name == "B"; });
		REQUIRE(a != namespaces.end());

		CHECK(a->m_namespaces.size() == 1);
		CHECK(b->m_namespaces.size() == 0);
		auto& aB = a->m_namespaces.back();
		checkEmpty(aB);

		SECTION("Named correctly") {
			CHECK(a->m_name == "A");
			CHECK(b->m_name == "B");
			CHECK(aB.m_name == "B");
		}

		SECTION("Nested correctly") {
			CHECK(aB.m_parent == "A");
			CHECK(b->m_parent.empty());
		}
	}
}
