#include "TestUtil/parse.hpp"
#include "TestUtil/types.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <variant>

TEST_CASE("Function taking a C-style function argument", "[functionTypes]") {
	auto code = R"(
double outerFunction(void(*argumentFunction)(int)) {
	argumentFunction(5);
	return 4.0;
}
)";
	CAPTURE(code);
	auto globalNS = TestUtil::parseString(code);
	REQUIRE(globalNS.m_functions.size() == 1);
	auto& f = globalNS.m_functions.back();
	REQUIRE(f.m_arguments.size() == 1);
	auto& arg = f.m_arguments.back();
	REQUIRE(arg.m_name == "argumentFunction");
	auto type = arg.m_type;
	CAPTURE(type.m_representation);
	// Can be expanded to one of:
	//  std::function<void(void)>
	//  std::function<void (void)>
	//  std::function<void()>
	//  std::function<void ()>
	REQUIRE(type.m_representation.find("void") != std::string::npos);
	REQUIRE(type.m_representation.find("(int)") != std::string::npos);

	auto& func = arg.m_type.m_type;
	auto funcType = std::get_if<IR::Type::Function>(&func);
	REQUIRE(funcType != nullptr);
	REQUIRE(funcType->m_representation != "");
}

TEST_CASE("Function taking a function argument", "[functionTypes]") {
	auto code = R"(
#include <functional>

void f(std::function<void()> arg);
)";
	CAPTURE(code);
	auto globalNS = TestUtil::parseString(code);
	REQUIRE(globalNS.m_functions.size() == 1);
	auto& f = globalNS.m_functions.back();
	REQUIRE(f.m_arguments.size() == 1);
	auto& arg = f.m_arguments.back();
	REQUIRE(arg.m_name == "arg");
	auto type = arg.m_type;
	CAPTURE(type.m_representation);
	// Can be expanded to one of:
	//  std::function<void(void)>
	//  std::function<void (void)>
	//  std::function<void()>
	//  std::function<void ()>
	REQUIRE(type.m_representation.find("std::function<void") !=
	        std::string::npos);

	auto& func = arg.m_type.m_type;
	auto funcType = std::get_if<IR::Type::Function>(&func);
	REQUIRE(funcType != nullptr);
	REQUIRE(funcType->m_representation != "");
}

