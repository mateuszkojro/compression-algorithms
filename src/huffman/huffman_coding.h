#ifndef COMPRESSION_ALGORITHMS_APPLICATION_SRC_HUFFMAN_HUFFMAN_CODING_H_
#define COMPRESSION_ALGORITHMS_APPLICATION_SRC_HUFFMAN_HUFFMAN_CODING_H_
#include "../Compressor.h"
#include "BinaryTree.h"
#include <chrono>
#include <queue>


class HuffmanCoding : public Compressor {
 public:
  TimeAndValue<void> Compress(const std::string &text) override;
  TimeAndValue<std::string> Decompress() override;
  size_t CompressedSize() override;
  ~HuffmanCoding() override = default;
 private:
  std::vector<bool> compressed_;
  std::queue<BinaryTreeNode *> base_node_;
};
#endif