#include "Helpers/commandLineArgs.hpp"
#include <catch2/catch.hpp>
#include <filesystem>

namespace {
std::string removeSubstring(std::string str, std::string const& subString) {
	// Search for the include flag substring in string
	auto pos = str.find(subString);

	if (pos != std::string::npos) {
		// If found then erase it from string
		str.erase(pos, subString.length());
	}
	return str;
}
}    // namespace

TEST_CASE("getCommandLineArgs returns a custom system include path",
          "[commandLineArgs]") {
	using path = std::filesystem::path;
	std::string myPath = (path("my") / path("custom") / path("path")).string();
	std::string systemInclude = "-isystem";
	auto args = Helpers::getCommandLineArgs({systemInclude + myPath});
	REQUIRE(!args.empty());
	for (auto arg : args) {
		CAPTURE(arg);
		if (auto include = removeSubstring(arg, systemInclude);
		    include != arg) {
			CAPTURE(include);
			REQUIRE(include == myPath);
		}
	}
}
