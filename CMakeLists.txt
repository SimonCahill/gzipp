cmake_minimum_required(VERSION 3.10)

project(gzipp LANGUAGES CXX DESCRIPTION "A header-only GZip implementation for modern C++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
add_library(procsys::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE include)

###
# Find zlib
###
find_package(ZLIB REQUIRED)

target_link_libraries(${PROJECT_NAME} INTERFACE
    ${ZLIB_LIBRARIES}
)

if (gzipp_BUILD_TESTS)
    include(GoogleTest)
    
    enable_testing()
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests/unit ${CMAKE_CURRENT_BINARY_DIR}/tests/unit)
endif()