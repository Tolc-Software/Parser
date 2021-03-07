#include "Helpers/windowsPathExtraction.hpp"
#include "Helpers/Utils/split.hpp"
#include <ctre.hpp>
#include <filesystem>
#include <fmt/format.h>
#include <optional>
#include <set>
#include <string>
#include <vector>

namespace Helpers {

static constexpr auto versionPattern =
    ctll::fixed_string {"^(\\d+\\.)?(\\d+\\.)?(\\d+\\.)?(\\*|\\d+)$"};

constexpr auto isVersion(std::string_view maybeVersion) noexcept {
	return ctre::match<versionPattern>(maybeVersion);
}

std::optional<std::string> getLatestVersionDirectory(std::string path) {
	std::set<std::string> versions;
	if (std::filesystem::exists(path)) {
		for (auto const& p : std::filesystem::directory_iterator(path)) {
			auto currentPath = p.path();
			auto fn = currentPath.filename().string();
			if (p.is_directory() && isVersion(fn)) {
				versions.insert(currentPath.string());
			}
		}
	}

	// NOTE: This assumes that if there are multiple versions,
	//       they are all of the same type
	//       (i.e. they all include the same combination of {major, minor, patch, prerelease})
	return versions.empty() ? path : *versions.rbegin();
}

/*
* Searches through path for versionPlaceHolder and switches it out for the largest version number available in the current filesystem
*/
std::optional<std::string>
getLatestVersionFromPath(std::string path,
                         std::string_view versionPlaceHolder) {
	auto splitPath = Helpers::Utils::split(path, versionPlaceHolder);

	// NOTE: Do not search for latest version in the last entry
	// Ex:
	//    C:/Hi/{versionPlaceHolder}/Bye
	//    Should only search for version in C:/Hi/
	//    Not in C:/Hi/{expandedVersion}/Bye
	std::string outPath = "";
	for (size_t i = 0; i < splitPath.size() - 1; i++) {
		if (auto directory =
		        getLatestVersionDirectory(outPath + splitPath[i])) {
			outPath = directory.value();
		}
	}
	// Add in the last part of the path (in the above example "/Bye")
	outPath += splitPath.back();
	if (std::filesystem::exists(outPath)) {
		return outPath;
	}
	return std::nullopt;
}

std::vector<std::string> filterExistingPathsWithLatestVersion(
    std::vector<std::string> const& potentialPaths,
    std::string_view versionPlaceholder) {
	std::vector<std::string> existingPaths;
	for (auto const& p : potentialPaths) {
		if (auto existingPath =
		        getLatestVersionFromPath(p, versionPlaceholder)) {
			existingPaths.push_back(existingPath.value());
		}
	}
	return existingPaths;
}

std::vector<std::string>
appendSystemIncludes(std::vector<std::string> const& paths) {
	std::vector<std::string> includes;
	for (auto const& path : paths) {
		includes.push_back("-isystem" + path);
	}
	return includes;
}
}    // namespace Helpers
