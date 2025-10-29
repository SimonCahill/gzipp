cmake_minimum_required(VERSION 3.20)

project(gzipp_test LANGUAGES CXX DESCRIPTION "A header-only GZip implementation for modern C++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GTest REQUIRED)

file(GLOB_RECURSE FILES FOLLOW_SYMLINKS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(gzipp_test
    ${FILES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    gzipp::gzipp
    ${GTEST_LIBRARIES}
)

gtest_discover_tests(${PROJECT_NAME})