#pragma once

#include <filesystem>
#include <fstream>
#include <limits>
#include <random>
#include <string>

namespace {
std::string getUniqueTempFilename(std::string const& ext = ".hpp") {
	// Use to build a random string
	std::random_device r;
	std::default_random_engine rEngine(r());
	std::uniform_int_distribution<> uniform(0, std::numeric_limits<int>::max());

	auto tempDir = std::filesystem::temp_directory_path();
	std::string tempFile = tempDir / std::to_string(uniform(rEngine));
	tempFile += ext;
	while (std::filesystem::exists(tempFile)) {
		tempFile = tempDir / std::to_string(uniform(rEngine));
		tempFile += ext;
	}
	return tempFile;
}
}    // namespace

namespace TestUtil {

// A file that removes itself on destruction
// NOTE: Located in temp directory
//       Will write over any file with the same name in this directory
class TemporaryFile {
public:
	TemporaryFile(std::string const& filename, std::string const& content)
	    : m_filename(filename) {
		std::ofstream file(m_filename);
		file << content;
		file.close();
	}
	~TemporaryFile() {
		if (std::filesystem::exists(m_filename)) {
			std::filesystem::remove(m_filename);
		}
	}

	// Implicitly converts to the filename
	operator std::filesystem::path() const {
		return m_filename;
	}

private:
	std::filesystem::path m_filename;
};

TemporaryFile writeToFile(std::string const& code,
                          std::string const& ext = ".hpp") {
	// TODO: Make the name unique
	//       so that more tests can use this at the same time
	TemporaryFile tf(getUniqueTempFilename(ext), code);
	return tf;
}

}    // namespace TestUtil
