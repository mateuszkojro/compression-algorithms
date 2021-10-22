cmake_minimum_required(VERSION 3.20)
project(test_application)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
include_directories(src)

link_libraries(compression_algorithms)

add_subdirectory(pm-stl)
include_directories(pm-stl)
link_libraries(pm_stl)

add_executable(test_app main.cpp main.cpp)
#target_link_libraries(test_app pm_stl)