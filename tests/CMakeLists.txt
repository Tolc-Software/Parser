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

# Defines create_catch2_test
include(SetupTest)

# The tests need fmt
run_conan(REQUIRES
  fmt/6.1.2)

find_package(fmt REQUIRED)

if(ENABLE_COVERAGE)
  include(CodeCoverage)

  # Use coverage of parser
  target_code_coverage(Parser)

  add_code_coverage_all_targets()
endif()

foreach(test
  arrays.cpp
  classes.cpp
  enums.cpp
  fields.cpp
  files.cpp
  tuples.cpp
  functions.cpp
  maps.cpp
  multipleClasses.cpp
  namespaces.cpp
  references.cpp
  dataStructs.cpp
  representation.cpp
  sets.cpp
  unordered_maps.cpp
  unordered_sets.cpp
  userDefined.cpp
  vectors.cpp

  Parser/Parse.cpp
  Helpers/Type/Matchers/container.cpp
  Helpers/Type/matchers.cpp
  Helpers/Utils/split.cpp
  Helpers/Utils/string.cpp
  Helpers/commandLineArgs.cpp
)
  # Ex: PybindProxy/moduleFile.cpp -> moduleFile
  get_filename_component(testName ${test} NAME_WLE)

  setup_test(
    TEST_NAME
    ${testName}
    SOURCE
    ${test}
    LIBRARIES
    fmt::fmt
    TestUtil)
endforeach()
