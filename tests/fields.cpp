#include "Helpers/walkIRStructure.hpp"
#include "TestUtil/compare.hpp"
#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"
#include "TestUtil/types.hpp"
#include <IR/ir.hpp>
#include <catch2/catch.hpp>
#include <variant>

TEST_CASE("Simple string member variable", "[fields]") {
	auto globalNS = TestUtil::parseString(R"(
#include <string>
class MyClass {
	std::string s;
};
		)", spdlog::level::debug);
	SECTION("Parser finds the variable") {
		REQUIRE(globalNS.m_structs.size() == 1);
		auto myClass = globalNS.m_structs[0];
		REQUIRE(myClass.m_memberVariables.size() == 1);
		auto& [access, variable] = myClass.m_memberVariables.back();
		CHECK(variable.m_name == "s");
		TestUtil::compare(variable.m_type, IR::BaseType::String);
		CHECK(variable.m_type.m_representation == "std::string");
	}
}

TEST_CASE("Member variable works with default modifier", "[fields]") {
	using IR::AccessModifier;
	for (auto [accessModifier, structure] :
	     {std::make_pair(AccessModifier::Private, std::string("class")),
	      std::make_pair(AccessModifier::Public, std::string("struct"))}) {
		auto globalNS =
		    TestUtil::parseString(structure + " MyStructure { int m_var; };");

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
		auto globalNS = TestUtil::parseString(
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
	auto globalNS = TestUtil::parseString(R"(
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
		REQUIRE_FALSE(variable.m_type.m_isConst);
		TestUtil::compare(variable.m_type, IR::BaseType::Int);
	}
}

TEST_CASE("Simple const member variable", "[fields]") {
	auto globalNS = TestUtil::parseString(R"(
class MyClass {
	int const i = 0;
};
		)");
	SECTION("Parser finds the variable") {
		REQUIRE(globalNS.m_structs.size() == 1);
		auto myClass = globalNS.m_structs[0];
		REQUIRE(myClass.m_memberVariables.size() == 1);
		auto& [access, variable] = myClass.m_memberVariables.back();
		CHECK(variable.m_name == "i");
		REQUIRE(variable.m_type.m_isConst);
		TestUtil::compare(variable.m_type, IR::BaseType::Int);
	}
}

TEST_CASE("Member variables of base types", "[fields]") {
	for (auto irType : TestUtil::getTypes()) {
		if (auto include = TestUtil::getIncludesIfNeeded(irType);
		    irType != IR::BaseType::Void) {
			auto type = TestUtil::getAsString(irType);
			// Example:
			//     #include <string>
			//     class MyClass { std::string m_member; };
			std::string code =
			    include + "\nclass MyClass { " + type + " m_member; };";
			auto globalNS = TestUtil::parseString(code);
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

TEST_CASE("Member variables of base types with using", "[fields]") {
	for (auto irType : TestUtil::getTypes()) {
		if (auto include = TestUtil::getIncludesIfNeeded(irType);
		    irType != IR::BaseType::Void) {
			auto type = TestUtil::getAsString(irType);
			// Example:
			//     #include <string>
			//     using MyWeirdType = std::string;
			//     class MyClass { MyWeirdType m_member; };
			std::string code = include + "\nusing MyWeirdType = " + type +
			                   ";\n" +
			                   "\nclass MyClass { MyWeirdType m_member; };";
			auto globalNS = TestUtil::parseString(code);
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

// TEST_CASE("Function with pointer argument", "[fields]") {
// 	auto globalNS = TestUtil::parseString(R"(
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
// 	auto globalNS = TestUtil::parseString(R"(
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
