#include "TestUtil/compare.hpp"
#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"
#include "TestUtil/types.hpp"
#include <IR/ir.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Function template parameters", "[templates]") {
	auto globalNS = TestUtil::parseString(R"(
template <typename T, typename U>
T myFun(U type) {
	return static_cast<T>(type);
}

template <>
int myFun(double type);
	)");

	REQUIRE(globalNS.m_functions.size() == 1);
	auto myFun = globalNS.m_functions[0];
	CHECK(myFun.m_name == "myFun<int, double>");
	CHECK(myFun.m_representation == "myFun<int, double>");
	TestUtil::compare(myFun.m_returnType, IR::BaseType::Int);
	REQUIRE(myFun.m_arguments.size() == 1);
	auto type = myFun.m_arguments[0];
	CHECK(type.m_name == "type");
	TestUtil::compare(type.m_type, IR::BaseType::Double);
}

TEST_CASE("Function template parameter", "[templates]") {
	auto globalNS = TestUtil::parseString(R"(
template <typename T>
T myFun(T type) {
	return type;
}

template <>
int myFun(int type);
	)");

	REQUIRE(globalNS.m_functions.size() == 1);
	auto myFun = globalNS.m_functions[0];
	CHECK(myFun.m_name == "myFun<int>");
	CHECK(myFun.m_representation == "myFun<int>");
	REQUIRE(myFun.m_arguments.size() == 1);
	auto type = myFun.m_arguments[0];
	CHECK(type.m_name == "type");
	TestUtil::compare(type.m_type, IR::BaseType::Int);
}

TEST_CASE("Class with constructor", "[templates]") {
	auto globalNS = TestUtil::parseString(R"(
template <typename T>
class MyClass {
public:
MyClass(T type);
};

template <>
class MyClass<int>;
	)");

	REQUIRE(globalNS.m_structs.size() == 1);
	auto myClass = globalNS.m_structs[0];
	CHECK(myClass.m_name == "MyClass<int>");
	CHECK(myClass.m_representation == "MyClass<int>");
	REQUIRE(!myClass.m_hasImplicitDefaultConstructor);
	REQUIRE(myClass.m_functions.size() == 1);
	auto& [access, myFun] = myClass.m_functions.back();
	REQUIRE(access == IR::AccessModifier::Public);
	REQUIRE(myFun.m_name == "MyClass<int>");
	REQUIRE(myFun.m_representation == "MyClass<int>::MyClass<int>");
	REQUIRE(myFun.m_arguments.size() == 1);
	auto& type = myFun.m_arguments.back();
	REQUIRE(type.m_name == "type");
	TestUtil::compare(type.m_type, IR::BaseType::Int);
}
TEST_CASE("Member function template parameters", "[templates]") {
	auto globalNS = TestUtil::parseString(R"(
template <typename T>
class MyClass {
public:
T myFun(T type) {
	return type;
}
};

template <>
class MyClass<int>;
	)");

	REQUIRE(globalNS.m_structs.size() == 1);
	auto myClass = globalNS.m_structs[0];
	CHECK(myClass.m_name == "MyClass<int>");
	CHECK(myClass.m_representation == "MyClass<int>");
	REQUIRE(myClass.m_hasImplicitDefaultConstructor);
	REQUIRE(myClass.m_functions.size() == 1);
	auto& [access, myFun] = myClass.m_functions.back();
	REQUIRE(access == IR::AccessModifier::Public);
	REQUIRE(myFun.m_name == "myFun");
	REQUIRE(myFun.m_representation == "MyClass<int>::myFun");
	TestUtil::compare(myFun.m_returnType, IR::BaseType::Int);
	REQUIRE(myFun.m_arguments.size() == 1);
	auto& type = myFun.m_arguments.back();
	REQUIRE(type.m_name == "type");
	TestUtil::compare(type.m_type, IR::BaseType::Int);
}

TEST_CASE("Multiple template parameters", "[templates]") {
	auto globalNS = TestUtil::parseString(R"(
template <typename T, typename U>
class MyClass {
public:
  T m_memberT;
  U m_memberU;
};

template <>
class MyClass<int, double>;
	)");

	REQUIRE(globalNS.m_structs.size() == 1);
	auto myClass = globalNS.m_structs[0];
	CHECK(myClass.m_name == "MyClass<int, double>");
	CHECK(myClass.m_representation == "MyClass<int, double>");
	REQUIRE(myClass.m_memberVariables.size() == 2);
	REQUIRE(myClass.m_hasImplicitDefaultConstructor);
	for (auto& [access, member] : myClass.m_memberVariables) {
		if (member.m_name == "m_memberT") {
			TestUtil::compare(member.m_type, IR::BaseType::Int);
		} else if (member.m_name == "m_memberU") {
			TestUtil::compare(member.m_type, IR::BaseType::Double);
		} else {
			// Something went wrong
			REQUIRE(false);
		}
		REQUIRE(access == IR::AccessModifier::Public);
	}
}

TEST_CASE("Class function with declarative instantiation", "[templates]") {
	auto globalNS = TestUtil::parseString(R"(
namespace MyNamespace {

template <typename T>
class MyClass {
public:
  T m_member;
};

template <>
class MyClass<int>;
}
	)");

	REQUIRE(globalNS.m_namespaces.size() == 1);
	auto& myNamespace = globalNS.m_namespaces[0];
	REQUIRE(myNamespace.m_structs.size() == 1);
	auto myClass = myNamespace.m_structs[0];
	REQUIRE(myClass.m_hasImplicitDefaultConstructor);
	CHECK(myClass.m_name == "MyClass<int>");
	CHECK(myClass.m_representation == "MyNamespace::MyClass<int>");
	REQUIRE(myClass.m_memberVariables.size() == 1);
	auto& [access, member] = myClass.m_memberVariables.back();
	REQUIRE(member.m_name == "m_member");
	TestUtil::compare(member.m_type, IR::BaseType::Int);
	REQUIRE(access == IR::AccessModifier::Public);
}

TEST_CASE("Class function with explicit instantiation", "[templates]") {
	auto globalNS = TestUtil::parseString(R"(
template <typename T>
class MyClass {
public:
  T m_member;
};

template <>
class MyClass<int> {
public:
  int m_member;
};
	)");
	REQUIRE(globalNS.m_structs.size() == 1);
	auto myClass = globalNS.m_structs[0];
	REQUIRE(myClass.m_hasImplicitDefaultConstructor);
	CHECK(myClass.m_name == "MyClass<int>");
	REQUIRE(myClass.m_memberVariables.size() == 1);
	auto& [access, member] = myClass.m_memberVariables.back();
	REQUIRE(member.m_name == "m_member");
	TestUtil::compare(member.m_type, IR::BaseType::Int);
	REQUIRE(access == IR::AccessModifier::Public);
}

TEST_CASE("Free function instantiations are parsed", "[templates]") {
	auto globalNS = TestUtil::parseString(R"(
template <typename T>
T getSomething(T something) {
        return something;
}

template <>
double getSomething(double d);

template <>
int getSomething(int i);
	)");
	REQUIRE(globalNS.m_functions.size() == 2);
	auto fun = globalNS.m_functions[0];
	for (auto fun : globalNS.m_functions) {
		REQUIRE(fun.m_arguments.size() == 1);
		if (fun.m_arguments.back().m_name == "d") {
			REQUIRE(fun.m_name == "getSomething<double>");
			TestUtil::compare(fun.m_returnType, IR::BaseType::Double);
			TestUtil::compare(fun.m_arguments.back().m_type,
			                  IR::BaseType::Double);
		} else if (fun.m_arguments.back().m_name == "i") {
			REQUIRE(fun.m_name == "getSomething<int>");
			TestUtil::compare(fun.m_returnType, IR::BaseType::Int);
			TestUtil::compare(fun.m_arguments.back().m_type, IR::BaseType::Int);
		} else {
			// Something went wrong
			REQUIRE(false);
		}
	}
}
