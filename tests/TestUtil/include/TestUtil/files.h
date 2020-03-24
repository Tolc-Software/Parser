#pragma once

#include <filesystem>
#include <fstream>
#include <string>

namespace TestUtil {

// A file that removes itself on destruction
// NOTE: Located in temp directory
//       Will write over any file with the same name in this directory
class TemporaryFile {
public:
	TemporaryFile(std::string const& filename, std::string const& content)
	    : m_filename(std::filesystem::temp_directory_path() / filename) {
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
	operator std::string() const {
		return m_filename;
	}

private:
	std::filesystem::path m_filename;
};

TemporaryFile writeToFile(std::string const& code) {
	// TODO: Make the name unique
	//       so that more tests can use this at the same time
	TemporaryFile tf("whatever.cpp", code);
	return tf;
}

}    // namespace TestUtil
