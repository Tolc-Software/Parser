#include "Parser/Parse.h"
#include "TestUtil/compare.h"
#include "TestUtil/finders.h"
#include "TestUtil/types.h"
#include <IR/ir.hpp>
#include <catch2/catch.hpp>
#include <variant>

TEST_CASE("Simple string member variable", "[fields]") {
	auto globalNS = Parser::parseString(R"(
#include <string>
class MyClass {
	std::string s;
};
		)");
	SECTION("Parser finds the variable") {
		REQUIRE(globalNS.m_structs.size() == 1);
		auto myClass = globalNS.m_structs[0];
		REQUIRE(myClass.m_memberVariables.size() == 1);
		auto& [access, variable] = myClass.m_memberVariables.back();
		CHECK(variable.m_name == "s");
		// TestUtil::compare(variable.m_type, IR::BaseType::Int);
	}
}

TEST_CASE("Member variable works with default modifier", "[fields]") {
	using IR::AccessModifier;
	for (auto [accessModifier, structure] :
	     {std::make_pair(AccessModifier::Private, std::string("class")),
	      std::make_pair(AccessModifier::Public, std::string("struct"))}) {
		auto globalNS =
		    Parser::parseString(structure + " MyStructure { int m_var; };");

		SECTION("Parser finds the function") {
			REQUIRE(globalNS.m_structs.size() == 1);
			auto structure = globalNS.m_structs[0];
			REQUIRE(structure.m_memberVariables.size() == 1);
			auto& [access, var] = structure.m_memberVariables.back();
			CHECK(var.m_name == "m_var");
			TestUtil::compare(var.m_type, IR::BaseType::Int);
			CHECK(access == accessModifier);
		}
	}
}

TEST_CASE("Member variable within class with modifier", "[fields]") {
	for (auto accessModifier : TestUtil::getAccessModifiers()) {
		auto globalNS = Parser::parseString(
		    "class MyClass { " + TestUtil::getAsString(accessModifier) +
		    ": double myDouble; };");
		SECTION("Parser finds the variable") {
			REQUIRE(globalNS.m_structs.size() == 1);
			auto myClass = globalNS.m_structs[0];
			REQUIRE(myClass.m_memberVariables.size() == 1);
			auto& [access, variable] = myClass.m_memberVariables.back();
			CHECK(variable.m_name == "myDouble");
			CHECK(access == accessModifier);
		}
	}
}

TEST_CASE("Simple member variable", "[fields]") {
	auto globalNS = Parser::parseString(R"(
class MyClass {
	int i;
};
		)");
	SECTION("Parser finds the variable") {
		REQUIRE(globalNS.m_structs.size() == 1);
		auto myClass = globalNS.m_structs[0];
		REQUIRE(myClass.m_memberVariables.size() == 1);
		auto& [access, variable] = myClass.m_memberVariables.back();
		CHECK(variable.m_name == "i");
		TestUtil::compare(variable.m_type, IR::BaseType::Int);
	}
}

TEST_CASE("Member variables of different types without includes", "[fields]") {
	for (auto irType : TestUtil::getTypes()) {
		// Remove the ones who require an include
		if (auto include = TestUtil::getIncludesIfNeeded(irType);
		    include.empty() && irType != IR::BaseType::Void) {
			auto type = TestUtil::getAsString(irType);
			std::string code = "class MyClass { " + type + " m_member; };";
			auto globalNS = Parser::parseString(code);
			// Print on error
			CAPTURE(code);
			CAPTURE(type);

			SECTION("Parser finds the variable") {
				REQUIRE(globalNS.m_structs.size() == 1);
				auto myClass = globalNS.m_structs[0];
				REQUIRE(myClass.m_memberVariables.size() == 1);
				auto& [access, variable] = myClass.m_memberVariables.back();
				CHECK(variable.m_name == "m_member");
				SECTION("with correct return type") {
					TestUtil::compare(variable.m_type, irType);
				}
			}
		}
	}
}

// TEST_CASE("Const variable", "[fields]") {
// 	auto globalNS = Parser::parseString(R"(
// void fun(int const i) {}
// 		)");
// 	SECTION("Parser finds the variable") {
// 		REQUIRE(globalNS.m_functions.size() == 1);
// 		auto fun = globalNS.m_functions[0];
// 		SECTION("finds the const argument") {
// 			REQUIRE(fun.m_arguments.size() == 1);
// 			auto& arg = fun.m_arguments.back();
// 			REQUIRE(arg.m_type.m_isConst);
// 		}
// 	}
// }

// TEST_CASE("Function with pointer argument", "[fields]") {
// 	auto globalNS = Parser::parseString(R"(
// void fun(int* i);
// 		)");
// 	SECTION("Parser finds the variable") {
// 		REQUIRE(globalNS.m_functions.size() == 1);
// 		auto fun = globalNS.m_functions[0];
// 		SECTION("finds the pointer argument") {
// 			REQUIRE(fun.m_arguments.size() == 1);
// 			auto& arg = fun.m_arguments.back();
// 			REQUIRE(arg.m_type.m_numPointers == 1);
// 		}
// 	}
// }

// TEST_CASE("Function with const pointer argument", "[fields]") {
// 	auto globalNS = Parser::parseString(R"(
// void fun(int const* i);
// 		)");
// 	SECTION("Parser finds the variable") {
// 		REQUIRE(globalNS.m_functions.size() == 1);
// 		auto fun = globalNS.m_functions[0];
// 		SECTION("finds the const pointer argument") {
// 			REQUIRE(fun.m_arguments.size() == 1);
// 			auto& arg = fun.m_arguments.back();
// 			REQUIRE(arg.m_type.m_numPointers == 1);
// 			REQUIRE(arg.m_type.m_isConst);
// 		}
// 	}
// }

// TEST_CASE("Function with const return value", "[fields]") {
// 	auto globalNS = Parser::parseString(R"(
// char const* fun();
// 		)");
// 	SECTION("Parser finds the variable") {
// 		REQUIRE(globalNS.m_functions.size() == 1);
// 		auto fun = globalNS.m_functions[0];
// 		SECTION("finds the const pointer return type") {
// 			auto returnChar =
// 			    std::get_if<IR::Type::Value>(&fun.m_returnType.m_type);
// 			REQUIRE(returnChar);
// 			CHECK(!returnChar->m_keyType.has_value());
// 			CHECK(returnChar->m_valueType == IR::BaseType::Char);
// 			CHECK(fun.m_returnType.m_numPointers == 1);
// 			REQUIRE(fun.m_returnType.m_isConst);
// 		}
// 	}
// }
