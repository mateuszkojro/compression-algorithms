cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

add_library(performance STATIC
        clock.h clock.cpp
        tracer.h tracer.cpp)
