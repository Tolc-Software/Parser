cmake_minimum_required(VERSION 3.17)

project(
  consumer
  VERSION 0.1
  LANGUAGES CXX)

set(FETCHCONTENT_FULLY_DISCONNECTED
    ON
    CACHE BOOL
          "Do not try to download dependencies (assume they are copied in)")
set(Parser_ROOT ${CMAKE_CURRENT_LIST_DIR}/build/parser)
find_package(Parser REQUIRED CONFIG PATHS ${Parser_ROOT} REQUIRED)

add_executable(consumer src/consumer.cpp)
target_link_libraries(consumer PRIVATE Tolc::Parser)
