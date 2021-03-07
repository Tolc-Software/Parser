#include "Helpers/windowsPathExtraction.hpp"
#include "Helpers/Utils/split.hpp"
#include "TestUtil/files.hpp"
#include <catch2/catch.hpp>
#include <filesystem>
#include <string>
#include <vector>

TEST_CASE("appendSystemIncludes appends system includes : )",
          "[windowsPathExtraction]") {
	std::string path = "SomePath";
	auto includes = Helpers::appendSystemIncludes({path});
	REQUIRE(includes.size() == 1);
	auto include = includes.front();
	REQUIRE(include == std::string("-isystem") + path);
}

TEST_CASE("Filter does not include the path if it isn't a version",
          "[windowsPathExtraction]") {
	std::string versionPlaceholder = "{LATEST_VERSION}";
	for (auto const& notAVersion :
	     {"Hello", "2020Stuff", "10.11.Hello", "v2020"}) {
		auto notValid = TestUtil::TemporaryDirectory(notAVersion);

		std::string path =
		    (notValid.getRootDirectory() / versionPlaceholder).string();
		auto filtered = Helpers::filterExistingPathsWithLatestVersion(
		    {path}, versionPlaceholder);

		CAPTURE(filtered.size());
		CAPTURE(path);
		// The temporary path still exists
		REQUIRE(!filtered.empty());
		auto p = filtered.front();
		// but it should not include any of the available directories since they are not a version
		std::filesystem::path notValidPath = notValid;
		REQUIRE(p != notValidPath.string());
	}
}

TEST_CASE("Filter between versions and not versions",
          "[windowsPathExtraction]") {
	std::string versionPlaceholder = "{LATEST_VERSION}";
	for (auto const& [actualVersion, notAVersion] :
	     {std::make_pair("11", "Hello"),
	      std::make_pair("2019", "2020Stuff"),
	      std::make_pair("10.11", "10.11.Hello"),
	      std::make_pair("14.29.29333", "")}) {
		auto notValid = TestUtil::TemporaryDirectory(notAVersion);
		std::filesystem::path latestVersion =
		    notValid.getRootDirectory() / actualVersion;
		std::filesystem::create_directories(latestVersion);

		std::string path =
		    (notValid.getRootDirectory() / versionPlaceholder).string();
		auto filtered = Helpers::filterExistingPathsWithLatestVersion(
		    {path}, versionPlaceholder);

		REQUIRE(filtered.size() == 1);
		REQUIRE(filtered.front() == latestVersion.string());
	}
}

TEST_CASE("Filter between versions of just numbers finds the latest version",
          "[windowsPathExtraction]") {
	std::string versionPlaceholder = "{LATEST_VERSION}";
	for (auto const& [latest, old] :
	     {std::make_pair("11", "10"),
	      std::make_pair("2019", "2017"),
	      std::make_pair("10.11", "10.10"),
	      /* Not supported: std::make_pair("10.10", "10.9"), */
	      std::make_pair("14.29.29333", "14.28.29333"),
	      std::make_pair("10.0.18362.1", "10.0.18362.0")}) {
		auto oldVersion = TestUtil::TemporaryDirectory(old);
		std::filesystem::path latestVersion =
		    oldVersion.getRootDirectory() / latest;
		std::filesystem::create_directories(latestVersion);

		std::string path =
		    (oldVersion.getRootDirectory() / versionPlaceholder).string();
		auto filtered = Helpers::filterExistingPathsWithLatestVersion(
		    {path}, versionPlaceholder);

		REQUIRE(filtered.size() == 1);
		REQUIRE(filtered.front() == latestVersion.string());
	}
}
