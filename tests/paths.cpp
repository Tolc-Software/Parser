#include "TestUtil/compare.hpp"
#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"
#include <IR/ir.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Filesystem::path is parsed", "[paths]") {
	std::string code = R"(
#include <filesystem>

namespace fs = std::filesystem;
class MyClass {
	std::filesystem::path m_member0;
	fs::path m_member1;
};
)";
	auto globalNS = TestUtil::parseString(code);
	auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
	using IR::BaseType;
	for (auto const& name : {"m_member0", "m_member1"}) {
		auto& member =
		    TestUtil::findMember(myClass, name, IR::AccessModifier::Private);
		REQUIRE(member.m_name == name);
		TestUtil::compare(member.m_type, IR::BaseType::FilesystemPath);
	}
}

