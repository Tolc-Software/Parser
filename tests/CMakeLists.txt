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

# Defines create_catch2_test
include(SetupTest)

setup_test(TEST_NAME
           functions
           SOURCE
           functions.cpp
           LIBRARIES
           TestUtil)
setup_test(TEST_NAME
           fields
           SOURCE
           fields.cpp
           LIBRARIES
           TestUtil)
setup_test(TEST_NAME
           namespaces
           SOURCE
           namespaces.cpp
           LIBRARIES
           TestUtil)
setup_test(TEST_NAME
           classes
           SOURCE
           classes.cpp
           LIBRARIES
           TestUtil)
setup_test(TEST_NAME
           split
           SOURCE
           Helpers/Utils/split.cpp)
