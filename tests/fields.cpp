#include "TestUtil/compare.hpp"
#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"
#include "TestUtil/types.hpp"
#include <IR/ir.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Fixed width type equivalents", "[fields]") {
	std::string code = R"(
#include <cstdint>

class MyClass {
	int8_t m_member0;
	int16_t m_member1;
	int32_t m_member2;
	int64_t m_member3;

	uint8_t m_member4;
	uint16_t m_member5;
	uint32_t m_member6;
	uint64_t m_member7;
};
)";
	auto globalNS = TestUtil::parseString(code);
	auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
	using IR::BaseType;
	for (auto [typeName, i] : {std::make_pair("int8_t", 0),
	                           std::make_pair("int16_t", 1),
	                           std::make_pair("int32_t", 2),
	                           std::make_pair("int64_t", 3),
	                           std::make_pair("uint8_t", 4),
	                           std::make_pair("uint16_t", 5),
	                           std::make_pair("uint32_t", 6),
	                           std::make_pair("uint64_t", 7)}) {
		auto name = "m_member" + std::to_string(i);
		auto& member =
		    TestUtil::findMember(myClass, name, IR::AccessModifier::Private);
		REQUIRE(member.m_name == name);
		REQUIRE(member.m_type.m_representation == typeName);
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

			REQUIRE(globalNS.m_structs.size() == 1);
			auto myClass = globalNS.m_structs[0];
			REQUIRE(myClass.m_memberVariables.size() == 1);
			auto& [access, variable] = myClass.m_memberVariables.back();
			CHECK(variable.m_name == "m_member");
			TestUtil::compare(variable.m_type, irType);
		}
	}
}

TEST_CASE("Simple string member variable", "[fields]") {
	auto globalNS = TestUtil::parseString(R"(
#include <string>
class MyClass {
	std::string s;
};
		)");
	REQUIRE(globalNS.m_structs.size() == 1);
	auto myClass = globalNS.m_structs[0];
	REQUIRE(myClass.m_memberVariables.size() == 1);
	auto& [access, variable] = myClass.m_memberVariables.back();
	CHECK(variable.m_name == "s");
	TestUtil::compare(variable.m_type, IR::BaseType::String);
	CHECK(variable.m_type.m_representation == "std::string");
}

TEST_CASE("Member variable works with default modifier", "[fields]") {
	using IR::AccessModifier;
	for (auto [accessModifier, structure] :
	     {std::make_pair(AccessModifier::Private, std::string("class")),
	      std::make_pair(AccessModifier::Public, std::string("struct"))}) {
		auto globalNS =
		    TestUtil::parseString(structure + " MyStructure { int m_var; };");

		REQUIRE(globalNS.m_structs.size() == 1);
		auto s = globalNS.m_structs[0];
		REQUIRE(s.m_memberVariables.size() == 1);
		auto& [access, var] = s.m_memberVariables.back();
		CHECK(var.m_name == "m_var");
		TestUtil::compare(var.m_type, IR::BaseType::Int);
		CHECK(access == accessModifier);
	}
}

TEST_CASE("Member variable within class with modifier", "[fields]") {
	for (auto accessModifier : TestUtil::getAccessModifiers()) {
		auto globalNS = TestUtil::parseString(
		    "class MyClass { " + TestUtil::getAsString(accessModifier) +
		    ": double myDouble; };");
		REQUIRE(globalNS.m_structs.size() == 1);
		auto myClass = globalNS.m_structs[0];
		REQUIRE(myClass.m_memberVariables.size() == 1);
		auto& [access, variable] = myClass.m_memberVariables.back();
		CHECK(variable.m_name == "myDouble");
		CHECK(access == accessModifier);
	}
}

TEST_CASE("Simple member variable", "[fields]") {
	auto globalNS = TestUtil::parseString(R"(
class MyClass {
	int i;
};
		)");
	REQUIRE(globalNS.m_structs.size() == 1);
	auto myClass = globalNS.m_structs[0];
	REQUIRE(myClass.m_memberVariables.size() == 1);
	auto& [access, variable] = myClass.m_memberVariables.back();
	CHECK(variable.m_name == "i");
	REQUIRE_FALSE(variable.m_type.m_isConst);
	TestUtil::compare(variable.m_type, IR::BaseType::Int);
}

TEST_CASE("Simple const member variable", "[fields]") {
	auto globalNS = TestUtil::parseString(R"(
class MyClass {
	int const i = 0;
};
		)");
	REQUIRE(globalNS.m_structs.size() == 1);
	auto myClass = globalNS.m_structs[0];
	REQUIRE(myClass.m_memberVariables.size() == 1);
	auto& [access, variable] = myClass.m_memberVariables.back();
	CHECK(variable.m_name == "i");
	REQUIRE(variable.m_type.m_isConst);
	TestUtil::compare(variable.m_type, IR::BaseType::Int);
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

			REQUIRE(globalNS.m_structs.size() == 1);
			auto myClass = globalNS.m_structs[0];
			REQUIRE(myClass.m_memberVariables.size() == 1);
			auto& [access, variable] = myClass.m_memberVariables.back();
			CHECK(variable.m_name == "m_member");
			TestUtil::compare(variable.m_type, irType);
		}
	}
}
