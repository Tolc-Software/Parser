include_guard()

# Defines create_catch2_test and get_catch2
include(${CMAKE_SOURCE_DIR}/cmake/Catch2Helpers.cmake)

find_package(Catch2 REQUIRED)

# Everything put in TestUtil/include is available
add_library(TestUtil INTERFACE)
target_include_directories(TestUtil INTERFACE TestUtil/include)

if(ENABLE_COVERAGE)
  include(${CMAKE_SOURCE_DIR}/cmake/CodeCoverage.cmake)

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
  complex.cpp
  constructorsDestructors.cpp
  dataStructs.cpp
  dependency.cpp
  deques.cpp
  docsClass.cpp
  docsEnums.cpp
  docsFunctions.cpp
  docsNamespaces.cpp
  docsVariables.cpp
  enums.cpp
  fields.cpp
  files.cpp
  forward_lists.cpp
  functionTypes.cpp
  functions.cpp
  globalVariables.cpp
  inheritence.cpp
  lists.cpp
  maps.cpp
  multimaps.cpp
  multipleClasses.cpp
  multisets.cpp
  namespaces.cpp
  operators.cpp
  optional.cpp
  pairs.cpp
  paths.cpp
  polymorphic.cpp
  priority_queues.cpp
  queues.cpp
  references.cpp
  representation.cpp
  sets.cpp
  smartPointers.cpp
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
  valarrays.cpp
  variant.cpp
  vectors.cpp)

  # Ex: tests/moduleFile.cpp -> moduleFile
  get_filename_component(testName ${test} NAME_WLE)

  add_executable(${testName} ${test})
  target_link_libraries(${testName} PRIVATE Catch2::Catch2WithMain Parser
                                            fmt::fmt spdlog::spdlog TestUtil)

  target_include_directories(${testName} PRIVATE ${PROJECT_SOURCE_DIR}/src
                                                 ${llvm_entry_INCLUDE_DIR})
  set_target_properties(${testName} PROPERTIES CXX_STANDARD_REQUIRED ON
                                               CXX_EXTENSIONS OFF)

  add_test(NAME ${testName} COMMAND ${testName})
  add_warnings(TARGET ${testName})
  add_options(TARGET ${testName})

  set(ENABLE_SANITIZER_ADDRESS TRUE)
  set(ENABLE_SANITIZER_MEMORY TRUE)
  enable_sanitizers(${testName})
endforeach()
