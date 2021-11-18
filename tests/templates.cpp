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

template int myFun<int, double>(double);
	)");

	auto& myFun = TestUtil::findFunction(globalNS, "myFun");
	CHECK(myFun.m_representation == "myFun<int, double>");
	REQUIRE(myFun.m_templateArguments.size() == 2);
	TestUtil::compare(myFun.m_templateArguments[0], IR::BaseType::Int);
	TestUtil::compare(myFun.m_templateArguments[1], IR::BaseType::Double);

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

template int myFun<int>(int);
	)");

	auto& myFun = TestUtil::findFunction(globalNS, "myFun");
	CHECK(myFun.m_representation == "myFun<int>");
	REQUIRE(myFun.m_templateArguments.size() == 1);
	TestUtil::compare(myFun.m_templateArguments[0], IR::BaseType::Int);

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

template class MyClass<int>;
	)");

	auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
	CHECK(myClass.m_representation == "MyClass<int>");
	REQUIRE(myClass.m_templateArguments.size() == 1);
	TestUtil::compare(myClass.m_templateArguments[0], IR::BaseType::Int);
	REQUIRE(!myClass.m_hasImplicitDefaultConstructor);
	auto& constructor =
	    TestUtil::findFunction(myClass, "MyClass", IR::AccessModifier::Public);
	REQUIRE(constructor.m_representation == "MyClass<int>::MyClass");
	REQUIRE(constructor.m_arguments.size() == 1);
	auto& type = constructor.m_arguments.back();
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

template class MyClass<int>;
	)");

	auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
	CHECK(myClass.m_representation == "MyClass<int>");
	REQUIRE(myClass.m_templateArguments.size() == 1);
	TestUtil::compare(myClass.m_templateArguments[0], IR::BaseType::Int);

	REQUIRE(myClass.m_hasImplicitDefaultConstructor);
	auto& myFun =
	    TestUtil::findFunction(myClass, "myFun", IR::AccessModifier::Public);
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

template class MyClass<int, double>;
	)");

	auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
	CHECK(myClass.m_representation == "MyClass<int, double>");
	REQUIRE(myClass.m_templateArguments.size() == 2);
	TestUtil::compare(myClass.m_templateArguments[0], IR::BaseType::Int);
	TestUtil::compare(myClass.m_templateArguments[1], IR::BaseType::Double);
	REQUIRE(myClass.m_memberVariables.size() == 2);
	REQUIRE(myClass.m_hasImplicitDefaultConstructor);
	auto& memberT =
	    TestUtil::findMember(myClass, "m_memberT", IR::AccessModifier::Public);
	TestUtil::compare(memberT.m_type, IR::BaseType::Int);

	auto& memberU =
	    TestUtil::findMember(myClass, "m_memberU", IR::AccessModifier::Public);
	TestUtil::compare(memberU.m_type, IR::BaseType::Double);
}

TEST_CASE("Class function with declarative instantiation", "[templates]") {
	auto globalNS = TestUtil::parseString(R"(
namespace MyNamespace {

template <typename T>
class MyClass {
public:
  T m_member;
};

template class MyClass<int>;
}
	)");

	auto& myNamespace = TestUtil::findNamespace(globalNS, "MyNamespace");

	auto& myClass = TestUtil::findStruct(myNamespace, "MyClass");
	REQUIRE(myClass.m_hasImplicitDefaultConstructor);
	CHECK(myClass.m_representation == "MyNamespace::MyClass<int>");
	REQUIRE(myClass.m_templateArguments.size() == 1);
	TestUtil::compare(myClass.m_templateArguments[0], IR::BaseType::Int);

	auto& member =
	    TestUtil::findMember(myClass, "m_member", IR::AccessModifier::Public);
	TestUtil::compare(member.m_type, IR::BaseType::Int);
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
	auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
	REQUIRE(myClass.m_hasImplicitDefaultConstructor);
	REQUIRE(myClass.m_templateArguments.size() == 1);
	TestUtil::compare(myClass.m_templateArguments[0], IR::BaseType::Int);

	auto& member =
	    TestUtil::findMember(myClass, "m_member", IR::AccessModifier::Public);
	TestUtil::compare(member.m_type, IR::BaseType::Int);
}

TEST_CASE("Free function instantiations are parsed", "[templates]") {
	auto globalNS = TestUtil::parseString(R"(
template <typename T>
T getSomething(T something) {
        return something;
}

template double getSomething(double d);

template int getSomething(int i);
	)");
	REQUIRE(globalNS.m_functions.size() == 2);
	for (auto fun : globalNS.m_functions) {
		REQUIRE(fun.m_name == "getSomething");
		REQUIRE(fun.m_arguments.size() == 1);
		REQUIRE(fun.m_arguments.back().m_name == "something");
		REQUIRE(fun.m_templateArguments.size() == 1);
		if (fun.m_representation == "getSomething<double>") {
			for (auto t : {fun.m_returnType,
			               fun.m_arguments.back().m_type,
			               fun.m_templateArguments[0]}) {
				TestUtil::compare(t, IR::BaseType::Double);
			}
		} else if (fun.m_representation == "getSomething<int>") {
			for (auto t : {fun.m_returnType,
			               fun.m_arguments.back().m_type,
			               fun.m_templateArguments[0]}) {
				TestUtil::compare(t, IR::BaseType::Int);
			}
		} else {
			// Something went wrong
			REQUIRE(false);
		}
	}
}
