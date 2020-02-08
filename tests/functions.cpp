#include "Parser/Parse.h"
#include "TestUtil/finders.h"
#include "TestUtil/types.h"
#include <IR/ir.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Function with const argument", "[functions]") {
	auto globalNS = Parser::parseString(R"(
void fun(int const i) {}
		)");
	SECTION("Parser finds the function") {
		REQUIRE(globalNS.m_functions.size() == 1);
		auto fun = globalNS.m_functions[0];
		SECTION("finds the const argument") {
			REQUIRE(fun.m_arguments.size() == 1);
			auto& arg = fun.m_arguments.back();
			REQUIRE(arg.m_qualifiers.size() == 1);
			auto& qual = arg.m_qualifiers.back();
			CHECK(qual == IR::Qualifier::Const);
		}
	}
}

TEST_CASE("Simple function", "[functions]") {
	auto globalNS = Parser::parseString(R"(
void fun() {}
		)");
	SECTION("Parser finds the function") {
		REQUIRE(globalNS.m_functions.size() == 1);
		auto fun = globalNS.m_functions[0];
		CHECK(fun.m_name == "fun");
		CHECK(fun.m_arguments.size() == 0);
		CHECK(fun.m_returnType == IR::Type::Void);
	}
}

TEST_CASE("Function with different returns without includes", "[functions]") {
	for (auto irType : TestUtil::getTypes()) {    //{IR::Type::String}) {
		// Remove the ones who require an include
		if (auto include = TestUtil::getIncludesIfNeeded(irType);
		    include.empty()) {
			auto type = TestUtil::getTypeAsString(irType);
			std::string code = include + type + " fun() { return " +
			                   TestUtil::getValidReturnForType(irType) + "; }";
			auto globalNS = Parser::parseString(code);
			// Print on error
			CAPTURE(code);
			CAPTURE(type);

			SECTION("Parser finds the function") {
				REQUIRE(globalNS.m_functions.size() == 1);
				auto fun = globalNS.m_functions[0];
				CHECK(fun.m_name == "fun");
				CHECK(fun.m_arguments.size() == 0);
				SECTION("with correct return type") {
					CHECK(fun.m_returnType == irType);
				}
			}
		}
	}
}

TEST_CASE("Function with arguments not requiring includes", "[functions]") {
	for (auto irType : TestUtil::getTypes()) {
		// Remove the ones who require an include
		if (auto include = TestUtil::getIncludesIfNeeded(irType);
		    include.empty()) {
			auto type = TestUtil::getTypeAsString(irType);
			if (type == "void") {
				// Named variables cannot have type void
				continue;
			}
			std::string code = "void fun(" + type + " myArg) { return; }";
			auto globalNS = Parser::parseString(code);
			// Print on error
			CAPTURE(code);
			CAPTURE(type);

			SECTION("Parser finds the function") {
				REQUIRE(globalNS.m_functions.size() == 1);
				auto fun = globalNS.m_functions[0];
				CHECK(fun.m_name == "fun");
				CHECK(fun.m_returnType == IR::Type::Void);
				SECTION("with correct argument") {
					REQUIRE(fun.m_arguments.size() == 1);
					auto& arg = fun.m_arguments.back();
					CHECK(arg.m_name == "myArg");
					CHECK(arg.m_type == irType);
				}
			}
		}
	}
}

TEST_CASE("Multiple arguments", "[functions]") {
	auto globalNS = Parser::parseString(R"(
void fun(int i, double d, char c);
		)");
	REQUIRE(globalNS.m_functions.size() == 1);
	auto& fun = globalNS.m_functions.back();
	REQUIRE(fun.m_arguments.size() == 3);
	for (auto [argName, type] : {std::make_pair("i", IR::Type::Int),
	                             std::make_pair("d", IR::Type::Double),
	                             std::make_pair("c", IR::Type::Char)}) {
		auto maybeArg = TestUtil::findWithName(argName, fun.m_arguments);
		REQUIRE(maybeArg.has_value());
		auto i = maybeArg.value();
		CHECK(i.m_type == type);
	}
}

// TEST_CASE("Pointer argument", "[functions]") {
// 	auto globalNS = Parser::parseString(R"(
// char fun(char* c) {
// 	return *c;
// }
// 		)");
// }

// TEST_CASE("Pointer argument with const", "[functions]") {
// 	auto globalNS = Parser::parseString(R"(
// char const fun(char const * c) {
// 	return *c;
// }
// 		)");
// }
