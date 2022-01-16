#ifndef TEST_APPLICATION_HUFFMAN_CODING_H
#define TEST_APPLICATION_HUFFMAN_CODING_H

#include <string>
#include <vector>
//#define _HAS_CXX17 true
#include <optional>
//#undef _HAS_CXX17 false
#include <utility>
#include "../Compressor.h"
#include "../bit_vector/BitVector.h"

struct ProbabilityValuePair {
  ProbabilityValuePair(double probability, char value) : probability(probability), value(value) {}
  double probability;
  char value;
};

class BinaryTree {
  // Implementation based on: https://en.wikipedia.org/wiki/Binary_tree#Arrays
  using Type = ProbabilityValuePair;
  using T = std::optional<Type>;
 public:
  enum class ChildType {
    LEFT = 0, // WARN: Numbers here are important - they are used in calculations
    RIGHT = 1
  };
  using NodeId = size_t;
  explicit BinaryTree(size_t depth = 0);

  [[nodiscard]] T GetNodeValue(NodeId parent_id) const;
  void SetNode(NodeId node_id, T value);
  void SetNode(NodeId node_id, const BinaryTree & tree, NodeId root_node);

  static NodeId RootNodeId();
  static std::pair<NodeId, NodeId> GetChildrenIds(NodeId parent_id);
  static NodeId GetChild(NodeId parent_id, ChildType type);

  NodeId InsertChild(NodeId parent_id, BinaryTree::ChildType type, T value);
  NodeId InsertTree(NodeId parent_id, BinaryTree::ChildType type, const BinaryTree &tree);

  bool HasValue(NodeId node_id) const;
  bool Exists(NodeId node_id);

  // FIXME: I should create a real iterator but I need to do other things first
  std::vector<NodeId> NodeIterator();

 private:
  void ResizeStorage(size_t new_size);
  std::vector<T> nodes_{};
};

// ? how to store the tree
class HuffmanCoding : public Compressor{
 public:
  TimeAndValue<void> Compress(const std::string &c_1) override;
  TimeAndValue<std::string> Decompress() override;
 private:
  mk::BitVector compressed_stream_;
  BinaryTree tree_;
};

#endif //TEST_APPLICATION_HUFFMAN_CODING_H
