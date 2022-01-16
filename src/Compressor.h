//
// Created by M.Kojro on 11/27/2021.
//

#ifndef TEST_APPLICATION_SRC_COMPRESSOR_H_
#define TEST_APPLICATION_SRC_COMPRESSOR_H_

#include <chrono>
#include <sstream>
#include <unordered_map>

#include "bit_vector/BitVector.h"

template<class T>
struct TimeAndValue {
  TimeAndValue(T value, std::chrono::microseconds time) : time(time), value(value) {}
  std::chrono::microseconds time;
  T value;
};

template<>
struct TimeAndValue<void> {
  explicit TimeAndValue(std::chrono::microseconds time) : time(time) {}
  std::chrono::microseconds time;
};

class Compressor {
 public:
  virtual TimeAndValue<void> Compress(const std::string &text) = 0;
  virtual TimeAndValue<std::string> Decompress() = 0;
  virtual size_t CompressedSize() = 0;
  virtual ~Compressor() = default;
};

#endif //TEST_APPLICATION_SRC_COMPRESSOR_H_
