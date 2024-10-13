![Ubuntu](https://github.com/Tolc-Software/Parser/workflows/Ubuntu/badge.svg) ![MacOS](https://github.com/Tolc-Software/Parser/workflows/MacOS/badge.svg) ![Windows](https://github.com/Tolc-Software/Parser/actions/workflows/windows.yml/badge.svg) ![Windows-debug-deploy](https://github.com/Tolc-Software/Parser/actions/workflows/windows-debug-deploy.yml/badge.svg)

# Parser #

Parser is the library for converting a header file of `C++` to the intermediate representation declared in the library [`IR::IR`](https://github.com/Tolc-Software/IntermediateRepresentation).

## Building

Use a recent LLVM `clang`.

Generate:

```sh
cmake -B build -S . -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=cmake/conan_provider.cmake
```

Build:

```sh
cmake --build build
```


## Installation ##

Use `FetchContent` in your `CMakeLists.txt` as follows:

```cmake
include(FetchContent)
FetchContent_Declare(
  Parser
  GIT_REPOSITORY git@github.com:Tolc-Software/Parser.git
  GIT_TAG main)

# Downloads and makes the parser library available
FetchContent_MakeAvailable(Parser)

# Link it to your target
target_link_libraries(your_target PRIVATE Tolc::Parser)
```

And you should get all the necessary components of the library.


## Usage ##

`Parser` exposes the header file [`Parser/Parse.h`](./include/Parser/Parse.hpp), which can be used as:

```cpp
#include <Parser/Parse.hpp>
#include <iostream>

int main() {
    // Returns a std::optional<IR::Namespace>
    auto maybeParsed = Parser::parseString(R"(
int f() {
    return 5;
}
    )");

    if (maybeParsed) {
        auto& globalNamespace = maybeParsed.value();
        // Will return "f"
        std::cout << globalNamespace.m_functions[0].m_name << '\n';
    }
}
```
