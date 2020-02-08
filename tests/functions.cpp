#include "Parser/Parse.h"
#include "TestUtil/types.h"
#include <IR/ir.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Function with argument", "[functions]") {
	auto globalNS = Parser::parseString(R"(
void fun(int i) {}
		)");
	SECTION("Parser finds the function") {
		REQUIRE(globalNS.m_functions.size() == 1);
		auto fun = globalNS.m_functions[0];
		CHECK(fun.m_name == "fun");
		CHECK(fun.m_returnType == IR::Type::Void);

		REQUIRE(fun.m_arguments.size() == 1);
		auto& arg = fun.m_arguments.back();
		CHECK(arg.m_name == "i");
		CHECK(arg.m_type == IR::Type::Int);
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

// TEST_CASE("With return type", "[functions]") {
// 	auto globalNS = Parser::parseString(R"(
// int fun() {
// 	return 5;
// }
// 		)");
// }

// TEST_CASE("Return type and simple argument", "[functions]") {
// 	auto globalNS = Parser::parseString(R"(
// int fun(int i) {
// 	return i;
// }
// 		)");
// }

// TEST_CASE("Return type with const qualifier", "[functions]") {
// 	auto globalNS = Parser::parseString(R"(
// int fun(int const i) {
// 	return i;
// }
// 		)");
// }

// TEST_CASE("Multiple return types", "[functions]") {
// 	auto globalNS = Parser::parseString(R"(
// int fun(const int i, double d) {
// 	return d + 5;
// }
// 		)");
// }

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
