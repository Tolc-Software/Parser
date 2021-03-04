cmake_minimum_required(VERSION 3.17)

project(
  consumer
  VERSION 0.1
  LANGUAGES CXX)

# Set /MT (or /MTd if debug) in MSVC-like compilers
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "Used to tell which runtime lib to link with." FORCE)

set(FETCHCONTENT_FULLY_DISCONNECTED
    ON
    CACHE BOOL
          "Do not try to download dependencies (assume they are copied in)")
set(Parser_ROOT ${CMAKE_CURRENT_LIST_DIR}/build/parser)
find_package(Parser REQUIRED CONFIG PATHS ${Parser_ROOT} REQUIRED)

add_executable(consumer src/consumer.cpp)
target_link_libraries(consumer PRIVATE Tolc::Parser)
