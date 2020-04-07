#include "IRProxy/IRData.hpp"
#include "TestUtil/getName.hpp"
#include "TestUtil/parse.hpp"
#include <catch2/catch.hpp>

TEST_CASE("Simple root representation", "[representation]") {
	using IRProxy::Structure;
	for (auto const& [structure, type] :
	     {std::make_pair("namespace Simple {}", Structure::Namespace),
	      std::make_pair("class Simple {};", Structure::Struct),
	      std::make_pair("int Simple();", Structure::Function)}) {
		auto globalNS = TestUtil::parseString(structure);
		CAPTURE(structure);
		auto [name, representation] =
		    TestUtil::getNameAndRepresentation(type, globalNS);
		REQUIRE(name == representation);
	}
}

TEST_CASE("Representation one namespace deep", "[representation]") {
	using IRProxy::Structure;
	for (auto const& [structure, type] :
	     {std::make_pair("namespace Simple {}", Structure::Namespace),
	      std::make_pair("class Simple {};", Structure::Struct),
	      std::make_pair("int Simple();", Structure::Function)}) {
		auto code =
		    "namespace OuterNamespace { " + std::string(structure) + " }";
		auto globalNS = TestUtil::parseString(code);
		CAPTURE(code);
		REQUIRE(globalNS.m_namespaces.size() == 1);
		auto& inner = globalNS.m_namespaces.back();
		auto [name, representation] =
		    TestUtil::getNameAndRepresentation(type, inner);
		REQUIRE((std::string("OuterNamespace::") + name) == representation);
	}
}

TEST_CASE("Representation one struct deep", "[representation]") {
	using IRProxy::Structure;
	// NOTE: A class cannot contain a namespace
	for (auto const& [structure, type] :
	     {std::make_pair("class Simple {};", Structure::Struct),
	      std::make_pair("int Simple();", Structure::Function)}) {
		auto code = "class OuterClass { " + std::string(structure) + " };";
		auto globalNS = TestUtil::parseString(code);
		CAPTURE(code);
		REQUIRE(globalNS.m_structs.size() == 1);
		auto& inner = globalNS.m_structs.back();
		auto [name, representation] =
		    TestUtil::getNameAndRepresentation(type, inner);
		REQUIRE((std::string("OuterClass::") + name) == representation);
	}
}
