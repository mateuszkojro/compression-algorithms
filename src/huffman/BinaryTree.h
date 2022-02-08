//
// Created by M.Kojro on 2/7/2022.
//

#ifndef COMPRESSION_ALGORITHMS_APPLICATION_SRC_HUFFMAN_BINARYTREE_H_
#define COMPRESSION_ALGORITHMS_APPLICATION_SRC_HUFFMAN_BINARYTREE_H_
#include <optional>

struct BinaryTreeNode {
 public:
  explicit BinaryTreeNode(std::optional<char> arg_value, uint64_t arg_freq) : freq(arg_freq), value(arg_value) {};
  BinaryTreeNode(uint64_t arg_value, BinaryTreeNode *left, BinaryTreeNode *right)
      : left(left), right(right), freq(arg_value) {}
  size_t size() {
    size_t size = 0;
    if (left) {
      size += left->size();
    }
    if (right) {
      size += right->size();
    }
    return size + 8;
  }
  uint64_t freq = 0;
  std::optional<char> value;
  BinaryTreeNode *left = nullptr;
  BinaryTreeNode *right = nullptr;

  ~BinaryTreeNode() {
    delete left;
    delete right;
  }
};

#endif //COMPRESSION_ALGORITHMS_APPLICATION_SRC_HUFFMAN_BINARYTREE_H_
