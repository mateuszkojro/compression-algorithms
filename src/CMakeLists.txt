cmake_minimum_required(VERSION 3.20)
project(compression_algorithms)

set(CMAKE_CXX_STANDARD 17)

add_library(compression_algorithms huffman/huffman_coding.cpp bit_vector/BitVector.cpp bit_vector/BitVector.h)
