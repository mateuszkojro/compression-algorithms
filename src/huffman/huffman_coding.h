//
// Created by dims_ on 13/10/2021.
//

#ifndef TEST_APPLICATION_HUFFMAN_CODING_H
#define TEST_APPLICATION_HUFFMAN_CODING_H

#include <string>
#include "../bit_vector/BitVector.h"

class BinaryTree {
  using T = char;
 public:
  T FindNonde(mk::BitVector bit_string);
  void InserrtNode(T value);
 private:
};

using T = char;
namespace mk {
//template<class T>
class Node {
 public:
  Node(T value, Node *right, Node *left) : value_(value), right_(right), left_(left) {}
 private:
  T value_;
  Node *left_;
  Node *right_;
};
};

// ? how to store the tree
class HuffmanCoding {
 public:
  static mk::BitVector Compress(const std::string &c_1);
  static std::string Decompress(const mk::BitVector &compressed_data);
 private:
  void AddNode(const std::string &data);
  mk::Node *CreateTree(const std::string &data);
};

#endif //TEST_APPLICATION_HUFFMAN_CODING_H
