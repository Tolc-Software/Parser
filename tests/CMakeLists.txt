include_guard()

# Defines create_catch2_test and get_catch2
include(${modules}/Catch2Helpers.cmake)

get_catch2(VERSION v2.13.8)

# Everything put in TestUtil/include is available
add_library(TestUtil INTERFACE)
target_include_directories(TestUtil INTERFACE TestUtil/include)

if(ENABLE_COVERAGE)
  include(${modules}/CodeCoverage.cmake)

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
  vectors.cpp

  )

  # Ex: PybindProxy/moduleFile.cpp -> moduleFile
  get_filename_component(testName ${test} NAME_WLE)

  create_catch2_test(
    TARGET
    ${testName}
    SOURCES
    ${test}
    INCLUDE
    ${PROJECT_SOURCE_DIR}/src
    LINK_LIBRARIES
    clangTooling
    clangBasic
    clangASTMatchers
    Parser
    fmt::fmt
    TestUtil)

  add_warnings(TARGET ${testName})
  add_options(TARGET ${testName})


  target_include_directories(${testName} SYSTEM
                             PRIVATE ${llvm_entry_INCLUDE_DIR})

  set(ENABLE_SANITIZER_ADDRESS TRUE)
  set(ENABLE_SANITIZER_MEMORY TRUE)
  enable_sanitizers(${testName})
endforeach()

