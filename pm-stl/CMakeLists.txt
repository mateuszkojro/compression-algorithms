cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(pm_stl)

add_subdirectory(clock)
add_subdirectory(coord)

include_directories(PUBLIC clock)
include_directories(PUBLIC coord)

add_library(pm_stl STATIC pm_include.h pm.cpp)
set_target_properties(pm_stl PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(pm_stl performance coord)

add_executable(test test.cpp)

target_link_libraries(test performance coord)