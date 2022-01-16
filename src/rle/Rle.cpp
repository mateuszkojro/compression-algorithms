//
// Created by M.Kojro on 11/27/2021.
//

#include <cassert>
#include "Rle.h"
TimeAndValue<void> Rle::Compress(const std::string &text) {
  std::vector<int> output;
  auto start = std::chrono::high_resolution_clock::now();
  assert(!text.empty());
  size_t counter = 1;
  char last_char = text[0];
  for (int i = 1; i < text.size(); i++) {
    char c = text[i];
    if (last_char == c) {
      counter++;
    } else {
      output.push_back(counter);
      output.push_back(last_char);

      counter = 1;
      last_char = c;
    }
  }
  // WARN: This might be wrong - fix for missing last letter
  output.push_back(counter);
  output.push_back(last_char);
  auto end = std::chrono::high_resolution_clock::now();
  compressed_data_ = output;
  return TimeAndValue<void>(
      std::chrono::duration_cast<std::chrono::microseconds>(end - start));
}

TimeAndValue<std::string> Rle::Decompress() {
  std::stringstream output;

  auto start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < compressed_data_.size(); i += 2) {
    size_t counter = compressed_data_[i];
    char c = compressed_data_[i + 1];
    while (counter--) {
      output << c;
    }
  }
  auto end = std::chrono::high_resolution_clock::now();

  return {output.str(),
          std::chrono::duration_cast<std::chrono::microseconds>(end - start)};
}

size_t Rle::CompressedSize() {
  return compressed_data_.size() * 8; // In bits
}
