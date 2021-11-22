#include "TestUtil/compare.hpp"
#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"
#include <IR/ir.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Filesystem::path is parsed", "[paths]") {
	std::string code = R"(
#include <array>
#include <string>

auto getS(std::array<std::string, 2> s) { return s; }
)";
	auto globalNS = TestUtil::parseString(code);
	auto& getS = TestUtil::findFunction(globalNS, "getS");
	auto& s = TestUtil::findArgument(getS, "s");
	TestUtil::compare(s.m_type, IR::ContainerType::Array);
}

