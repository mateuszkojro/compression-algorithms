cmake_minimum_required(VERSION 3.20)
project(compression_algorithms)

set(CMAKE_CXX_STANDARD 17)

add_library(compression_algorithms
        Compressor.h
        bit_vector/BitVector.cpp bit_vector/BitVector.h
        huffman/huffman_coding.cpp huffman/huffman_coding.h
        rle/Rle.cpp rle/Rle.h
        lzw/lzw.cpp lzw/lzw.h
        )
