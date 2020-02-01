include_guard()

include(FetchContent)
FetchContent_Declare(Catch2
                     GIT_REPOSITORY git@github.com:catchorg/Catch2.git
                     GIT_TAG v2.11.0)

FetchContent_MakeAvailable(Catch2)

# These are needed to use create_catch2_test
set(Catch2_FOUND TRUE)
set(Catch2_INCLUDE_DIRS ${Catch2_SOURCE_DIR}/single_include)

# Defines create_catch2_test
include(SetupTest)

setup_test(TEST_NAME functions SOURCE functions.cpp)
setup_test(TEST_NAME namespaces SOURCE namespaces.cpp)
setup_test(TEST_NAME split SOURCE Helpers/Utils/split.cpp)
