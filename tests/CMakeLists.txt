include_guard()

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY git@github.com:catchorg/Catch2.git
  GIT_TAG v2.12.1)

FetchContent_MakeAvailable(Catch2)

# Add custom scripts
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

# Everything put in TestUtil/include is available
add_library(TestUtil INTERFACE)
target_include_directories(TestUtil INTERFACE TestUtil/include)

# These are needed to use create_catch2_test
set(Catch2_FOUND TRUE)
set(Catch2_INCLUDE_DIRS ${Catch2_SOURCE_DIR}/single_include)

# Defines create_catch2_test
include(${PROJECT_SOURCE_DIR}/cmake/SetupTest.cmake)

if(ENABLE_COVERAGE)
  include(cmake/CodeCoverage.cmake)

  # Use coverage of parser
  target_code_coverage(Parser)

  add_code_coverage_all_targets()
endif()

foreach(
  test

  Helpers/Type/Matchers/container.cpp
  Helpers/Type/matchers.cpp
  Helpers/Utils/split.cpp
  Helpers/Utils/string.cpp
  Helpers/commandLineArgs.cpp
  Parser/Parse.cpp
  Parser/Windows/systemIncludeHelper.cpp
  arrays.cpp
  autos.cpp
  classes.cpp
  dataStructs.cpp
  deques.cpp
  enums.cpp
  fields.cpp
  files.cpp
  forward_lists.cpp
  functionTypes.cpp
  functions.cpp
  globalVariables.cpp
  lists.cpp
  maps.cpp
  multimaps.cpp
  multipleClasses.cpp
  multisets.cpp
  namespaces.cpp
  optional.cpp
  paths.cpp
  priority_queues.cpp
  queues.cpp
  references.cpp
  representation.cpp
  sets.cpp
  stacks.cpp
  statics.cpp
  strings.cpp
  templates.cpp
  tuples.cpp
  unordered_maps.cpp
  unordered_multimaps.cpp
  unordered_multisets.cpp
  unordered_sets.cpp
  userDefined.cpp
  variant.cpp
  vectors.cpp

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
