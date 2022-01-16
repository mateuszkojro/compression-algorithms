cmake_minimum_required(VERSION 3.20)
project(compression-algorithms-application)
set(CMAKE_CXX_STANDARD 17)


add_subdirectory(src)
include_directories(src)

link_libraries(compression_algorithms)
set(MATI true)

#add_subdirectory(pm-stl)
#include_directories(pm-stl)
#link_libraries(pm_stl)

#find_package(ROOT REQUIRED COMPONENTS Core Gui)
#include_directories(ROOT_INCLUDE_DIRS)

add_executable(compression-algortihms-app main.cpp main.cpp Harness.cpp Harness.h)
#target_link_libraries(test_app PUBLIC ROOT::Core ROOT::Gui)

#target_link_libraries(test_app pm_stl)
