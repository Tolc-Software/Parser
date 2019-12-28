include_guard()

include(FetchContent)
FetchContent_Declare(Catch2
                     GIT_REPOSITORY git@github.com:catchorg/Catch2.git
                     GIT_TAG v2.11.0)

FetchContent_MakeAvailable(Catch2)

# To use create_catch2_test these are needed
set(Catch2_FOUND TRUE)
set(Catch2_INCLUDE_DIRS ${Catch2_SOURCE_DIR}/single_include)

# Helper function
function(setup_test test_target)
  # NOTE: Assumes the source of the target has the same name as the target dot
  # cpp
  create_catch2_test(TARGET
                     ${test_target}
                     SOURCES
                     ${test_target}.cpp
                     INCLUDE
                     ../src
                     LINK_LIBRARIES
                     project_warnings
                     project_options
                     clangTooling
                     clangBasic
                     clangASTMatchers
                     Parser)
  target_include_directories(${test_target} SYSTEM
                             PRIVATE ${llvm_entry_INCLUDE_DIR})
  add_test(NAME ${test_target} COMMAND ${test_target})
endfunction()

# Ignore warnings from clang/ as if they were system headers.
# NOTE: This is only available on clang

setup_test(functions)
