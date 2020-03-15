include_guard()

include(FetchContent)
FetchContent_Declare(Catch2
                     GIT_REPOSITORY git@github.com:catchorg/Catch2.git
                     GIT_TAG v2.11.0)

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

# Integration tests are located at the root
foreach(integrationTest functions fields namespaces classes userDefined)
  setup_test(TEST_NAME
             ${integrationTest}
             SOURCE
             ${integrationTest}.cpp
             LIBRARIES
             TestUtil)
endforeach()

setup_test(TEST_NAME
           split
           SOURCE
           Helpers/Utils/split.cpp)
setup_test(TEST_NAME
           includes
           SOURCE
           Helpers/includes.cpp)
