#include "huffman_coding.h"

#include <iostream>
#include <unordered_map>
#include <algorithm>

mk::BitVector HuffmanCoding::Compress(const std::string &input_data) {

  std::string data = input_data;
  mk::BitVector output_data;

  // Count frequency of each char
  std::unordered_map<char, unsigned> frequency_map;
  for (char c: data) {
    frequency_map[c]++;
  }

  // Sort by frequency
  std::sort(data.begin(), data.end(), [&frequency_map](char c_1, char c_2) {
    return frequency_map[c_1] > frequency_map[c_2];
  });



  return mk::BitVector();
}
std::string HuffmanCoding::Decompress(const mk::BitVector &compressed_data) {
  return std::string();
}



mk::Node *HuffmanCoding::CreateTree(const std::string &data) {
  return nullptr;
}
void HuffmanCoding::AddNode(const std::string &data) {

}

