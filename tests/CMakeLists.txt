include_guard()

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY git@github.com:catchorg/Catch2.git
  GIT_TAG v2.12.1)

FetchContent_MakeAvailable(Catch2)

# Everything put in TestUtil/include is available
add_library(TestUtil INTERFACE)
target_include_directories(TestUtil INTERFACE TestUtil/include)

# These are needed to use create_catch2_test
set(Catch2_FOUND TRUE)
set(Catch2_INCLUDE_DIRS ${Catch2_SOURCE_DIR}/single_include)

include(CodeCoverage)

# Defines create_catch2_test
include(SetupTest)

# Use coverage of parser
target_code_coverage(Parser)

add_code_coverage_all_targets()

foreach(test
  multipleClasses.cpp
  files.cpp
  containers.cpp
  functions.cpp
  fields.cpp
  namespaces.cpp
  classes.cpp
  userDefined.cpp
  representation.cpp
  references.cpp

  Helpers/Type/matchers.cpp
  Helpers/Utils/split.cpp
  Helpers/includes.cpp
  Helpers/Utils/string.cpp
  Helpers/Type/Matchers/container.cpp
)
  # Ex: PybindProxy/moduleFile.cpp -> moduleFile
  get_filename_component(testName ${test} NAME_WLE)

  setup_test(
    TEST_NAME
    ${testName}
    SOURCE
    ${test}
    LIBRARIES
    CONAN_PKG::fmt
    TestUtil)
endforeach()
