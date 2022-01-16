//
// Created by M.Kojro on 11/27/2021.
//

#include <cassert>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "lzw.h"
bool map_has_element(const std::unordered_map<std::string, size_t> &map,
                     const std::string &elem) {
  return map.find(elem) != map.end();
}

TimeAndValue<void> Lzw::Compress(const std::string &s1) {
  auto start = std::chrono::high_resolution_clock::now();

  std::unordered_map<std::string, int> table;
  for (int i = 0; i <= 255; i++) {
    std::string ch;
    ch += char(i);
    table[ch] = i;
  }

  std::string p, c;
  p += s1[0];

  int code = 256;

  std::vector<int> output_code;

  for (int i = 0; i < s1.size(); i++) {
    if (i != s1.size() - 1) {
      c += s1[i + 1];
    }
    if (table.find(p + c) != table.end()) {
      p += c;
    } else {
      output_code.push_back(table[p]);
      table[p + c] = code;
      code++;
      p = c;
    }
    c = "";
  }
  output_code.push_back(table[p]);

  compressed_data_ = output_code;

  auto end = std::chrono::high_resolution_clock::now();

  return TimeAndValue<void>(
      std::chrono::duration_cast<std::chrono::microseconds>(end - start));
}

TimeAndValue<std::string> Lzw::Decompress() {
  std::string decompressed;
  auto start = std::chrono::high_resolution_clock::now();

  std::unordered_map<int, std::string> table;

  for (int i = 0; i <= 255; i++) {
    std::string ch;
    ch += char(i);
    table[i] = ch;
  }

  auto &data = compressed_data_;

  int old = data[0];
  int n = 0;

  std::string s = table[old];
  std::string c;

  c += s[0];

  decompressed += s;
  int count = 256;
  for (int i = 0; i < data.size() - 1; i++) {
    n = data[i + 1];
    if (table.find(n) == table.end()) {
      s = table[old];
      s += c;
    } else {
      s = table[n];
    }
    decompressed += s;
    c = "";
    c += s[0];
    table[count] = table[old] + c;
    count++;
    old = n;
  }

  auto end = std::chrono::high_resolution_clock::now();

  return {decompressed,
          std::chrono::duration_cast<std::chrono::microseconds>(end - start)};
}

size_t Lzw::CompressedSize() {
  return compressed_data_.size() * 2 * 8;// In bits
}
