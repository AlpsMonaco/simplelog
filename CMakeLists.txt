cmake_minimum_required(VERSION 3.13)

project(simplelog)

set(CMAKE_CXX_STANDARD 17)

add_library(simplelog
    ${PROJECT_SOURCE_DIR}/simple_log.hpp
)

set_target_properties(simplelog PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(${PROJECT_SOURCE_DIR}/tests)