#pragma once

#include <string>
#include <vector>

namespace Helpers {

/**
* Return what command line arguments to use in the internal clang instance
* This includes things like, interpret everything as C++ regardless of file endings
* and paths to system headers.
* If the system includes input is empty, it will use CMake generated hardcoded paths to LLVM
* The systemIncludes should be on the format {"-isystem/path/one", "-isystem/path/two"}
*/
std::vector<std::string>
getCommandLineArgs(std::vector<std::string> const& systemIncludes = {});

}    // namespace Helpers
