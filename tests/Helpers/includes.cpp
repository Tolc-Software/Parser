#include "Helpers/includes.h"
#include <catch2/catch.hpp>
#include <filesystem>

namespace {
std::string removeIncludeFlag(std::string path) {
	// Search for the include flag substring in string
	std::string toErase = "-isystem";
	auto pos = path.find(toErase);

	// Should always exist
	REQUIRE(pos != std::string::npos);
	if (pos != std::string::npos) {
		// If found then erase it from string
		path.erase(pos, toErase.length());
	}
	return path;
}
}    // namespace

TEST_CASE("getSystemIncludes returns a path that exists", "[includes]") {
	auto includes = Helpers::getSystemIncludes();
	REQUIRE(!includes.empty());
	for (auto path : includes) {
		CAPTURE(path);
		path = removeIncludeFlag(path);
		CHECK(std::filesystem::exists(path));
	}
}
