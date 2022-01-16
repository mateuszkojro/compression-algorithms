//
// Created by M.Kojro on 11/27/2021.
//

#ifndef TEST_APPLICATION_SRC_LZW_LZW_H_
#define TEST_APPLICATION_SRC_LZW_LZW_H_

#include <unordered_map>
#include <codecvt>
#include <vector>

#include "../Compressor.h"

class Lzw : public Compressor {
  TimeAndValue<void> Compress(const std::string &s1) override;
  TimeAndValue<std::string> Decompress() override;
  size_t CompressedSize() override;
 private:
  // Character dictionary
  std::vector<int> compressed_data_;
};

#endif //TEST_APPLICATION_SRC_LZW_LZW_H_
