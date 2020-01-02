# Parser #

Parser is the library for converting a header file of `C++` to the intermediate representation declared in the library `IR::IR`

## Installation ##

Use `FetchContent` in your `CMakeLists.txt` as follows:

```cmake
include(FetchContent)
FetchContent_Declare(
  Parser
  GIT_REPOSITORY git@github.com:srydell/Parser.git
  GIT_TAG master)

# Downloads and makes the parser library available
FetchContent_MakeAvailable(Parser)
```

## Usage ##

Link against it in your `CMakeLists.txt` as follows:

```cmake
target_link_libraries(your_target PRIVATE Parser)
```

And you should get all the necessary components of the library.


## Structure ##

`Frontend => Consumer => Visitor`

The `Visitor` contains the parsing functions `VisitDecl(Decl* decl)` where a `Decl` is a `C++` parsed class. This could for example be a `FunctionDecl` containing all the information about a function in the given source.
