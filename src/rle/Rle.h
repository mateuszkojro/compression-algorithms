//
// Created by M.Kojro on 11/27/2021.
//

#ifndef TEST_APPLICATION_SRC_RLE_RLE_H_
#define TEST_APPLICATION_SRC_RLE_RLE_H_

#include <sstream>

#include "../Compressor.h"

class Rle : public Compressor {
 public:
  TimeAndValue<void> Compress(const std::string &text) override;
  TimeAndValue<std::string> Decompress() override;
  size_t CompressedSize() override;

 private:
  std::vector<int> compressed_data_;
};

#endif //TEST_APPLICATION_SRC_RLE_RLE_H_
