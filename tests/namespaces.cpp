#include "Helpers/namespaceHelpers.h"
#include "Parser/Parse.h"
#include "catch2/catch.hpp"
#include <algorithm>
#include <string_view>

namespace {
void checkEmpty(IR::Namespace& ns) {
	CAPTURE(ns.m_name);

	CHECK(ns.m_constants.empty());
	CHECK(ns.m_functions.empty());
	CHECK(ns.m_structs.empty());
}
}    // namespace

TEST_CASE("Single namespace", "[namespaces]") {
	auto namespaces = Parser::parseString("namespace Test {}");
	SECTION("Parser finds an empty namespace named Test") {
		REQUIRE(namespaces.size() == 1);
		auto& testNs = namespaces[0];

		checkEmpty(testNs);

		REQUIRE(testNs.m_name == "Test");
	}
}

TEST_CASE("Two namespaces", "[namespaces]") {
	auto namespaces = Parser::parseString(R"(
namespace Test0 {}
namespace Test1 {}
		)");
	SECTION("Parser finds two empty namespaces") {
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
	auto namespaces = Parser::parseString(R"(
namespace ParentNamespace {
	namespace ChildNamespace {}
}
		)");
	SECTION("Parser finds the empty root namespace") {
		REQUIRE(namespaces.size() == 1);
		auto& ns = namespaces.back();
		checkEmpty(ns);

		// It has one child
		REQUIRE(ns.m_children.size() == 1);
		auto& child = ns.m_children.back();
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

TEST_CASE("Three nested namespaces", "[namespaces]") {
	auto namespaces = Parser::parseString(R"(
namespace ParentNamespace {
	namespace ChildNamespace {
		namespace GrandchildNamespace {}
	}
}
		)");
	SECTION("Parser finds the three empty namespaces") {
		REQUIRE(namespaces.size() == 1);
		auto& root = namespaces.back();
		checkEmpty(root);

		REQUIRE(root.m_children.size() == 1);
		auto& child = root.m_children.back();
		checkEmpty(child);

		REQUIRE(child.m_children.size() == 1);
		auto& grandchild = child.m_children.back();
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
