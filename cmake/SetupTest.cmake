include_guard()
# Requires other scripts
include(Catch2Helpers)
include(CodeCoverage)

# Helper function to create a test
function(setup_test)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues TEST_NAME SOURCE)
  set(multiValues LIBRARIES)
  # Process the arguments passed in can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})

  get_filename_component(project_dir ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)
  create_catch2_test(
    TARGET
    ${ARG_TEST_NAME}
    SOURCES
    ${ARG_SOURCE}
    INCLUDE
    ${project_dir}/src
    LINK_LIBRARIES
    project_warnings
    project_options
    clangTooling
    clangBasic
    clangASTMatchers
    Parser
    ${ARG_LIBRARIES})
  target_include_directories(${ARG_TEST_NAME} SYSTEM
                             PRIVATE ${llvm_entry_INCLUDE_DIR})

  # Assumes CodeCoverage.cmake is included Adds code coverage to the test and
  # adds it to the ccov-all target
  target_code_coverage(${ARG_TEST_NAME} ALL)

  add_test(NAME ${ARG_TEST_NAME} COMMAND ${ARG_TEST_NAME})
endfunction()
