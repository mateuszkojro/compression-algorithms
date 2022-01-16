#include "huffman_coding.h"

#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <cassert>

// WARN: This function definitely can be optimized
TimeAndValue<void> HuffmanCoding::Compress(const std::string &input_data) {

  auto start = std::chrono::high_resolution_clock::now();
  std::string data = input_data; // Copy here
  mk::BitVector output_data;

  // TODO: Instead of using an unordered map and then sorting we could just use std::map
  // Count frequency of each char
  std::unordered_map<char, unsigned> frequency_map;
  for (char c: data) {
    frequency_map[c]++;
  }

  // Sort by frequency
  std::sort(data.begin(), data.end(), [&frequency_map](char c_1, char c_2) {
    return frequency_map[c_1] > frequency_map[c_2];
  });

  std::vector<ProbabilityValuePair> pairs;
  pairs.reserve(data.size());
  for (size_t i = 0; i < data.size(); i++) {
    // Items are sorted the way that the item that occurs the most is first on the list so the probability is p = size - i
    double probability = data.size() - i; // WARNING: For rly huge strings this might bite us
    pairs.emplace_back(probability, data[i]);
  }

  BinaryTree tree(std::ceil(std::log2(frequency_map.size())));

  size_t last_pair_start = pairs.size() - 1;
  size_t itr = 0;
  tree.SetNode(BinaryTree::RootNodeId(),
               ProbabilityValuePair(pairs[0].probability + pairs[1].probability, '\0'));

  tree.InsertChild(0, BinaryTree::ChildType::LEFT, pairs[0]);
  tree.InsertChild(0, BinaryTree::ChildType::RIGHT, pairs[1]);

  std::vector<BinaryTree> trees;
  trees.reserve(pairs.size());

  for (auto pv: pairs) {
    BinaryTree inner_tree;
    inner_tree.SetNode(BinaryTree::RootNodeId(), pv);
    trees.push_back(inner_tree);
  }

  while (trees.size() > 1) {
    size_t last_index = trees.size() - 1;

    auto &current_tree = trees[last_index - 1];
    auto[left, right] = BinaryTree::GetChildrenIds(BinaryTree::RootNodeId());

    current_tree.InsertTree(left, BinaryTree::ChildType::LEFT, current_tree);
    current_tree.InsertTree(right, BinaryTree::ChildType::RIGHT, trees[last_index]);
    trees.pop_back();
  }
  //  tree_ = trees[0];
  tree_ = BinaryTree(trees[0]); // Copy the resulting tree to the member filed

  for (char c: data) {

  }
  auto end = std::chrono::high_resolution_clock::now();
  return TimeAndValue<void>(
      std::chrono::duration_cast<std::chrono::microseconds>(end - start));
}

TimeAndValue<std::string> HuffmanCoding::Decompress() {
  BinaryTree::NodeId current_node = 0;
  std::string decompressed_string;
  decompressed_string.reserve(compressed_stream_.Size() / 8);
  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < compressed_stream_.Size(); i++) {
    if (auto maybe_node_value = tree_.GetNodeValue(current_node); maybe_node_value.has_value()) {
      decompressed_string.push_back(maybe_node_value.value().value);
      current_node = BinaryTree::RootNodeId();
    }
    // Traverse the tree
    current_node = BinaryTree::GetChild(current_node, static_cast<BinaryTree::ChildType>(compressed_stream_.GetBit(i)));
  }

  auto end = std::chrono::high_resolution_clock::now();
  return {decompressed_string,
          std::chrono::duration_cast<std::chrono::microseconds>(end - start)};
}

BinaryTree::BinaryTree(size_t depth) {
  size_t vector_size = 2 * depth + 1; // One node means depth 0
  ResizeStorage(vector_size);
}

void BinaryTree::ResizeStorage(size_t new_size) {
  nodes_.reserve(new_size);
  while (nodes_.size() < new_size) {
    nodes_.emplace_back(std::nullopt);
  }
}

BinaryTree::NodeId BinaryTree::InsertChild(NodeId parent_id, BinaryTree::ChildType type, T value) {
  // FIXME: Check if the tree needs to be resized and do it if so
  NodeId target_adress = GetChild(parent_id, type);
  if (target_adress >= nodes_.size()) {
    /*
     * log2(x) - calculates to which power we need to rise 2 to get enough space
     * then we need to increase it (because of 0 indexing)
     * and ceil to get next integer. Then we just raise 2 to that power and get new array size
     */
    ResizeStorage(static_cast<size_t>(
                      std::pow(2, std::ceil(std::log2(target_adress) + 1.0))));
  }

  SetNode(target_adress, value);
  return target_adress;
}

std::pair<BinaryTree::NodeId, BinaryTree::NodeId> BinaryTree::GetChildrenIds(BinaryTree::NodeId parent_id) {
  return std::make_pair(
      GetChild(parent_id, ChildType::LEFT),
      GetChild(parent_id, ChildType::RIGHT)
  );
}

BinaryTree::T BinaryTree::GetNodeValue(BinaryTree::NodeId node_id) const {
  return nodes_[node_id];
}

BinaryTree::NodeId BinaryTree::GetChild(BinaryTree::NodeId parent_id, BinaryTree::ChildType type) {
  // Left node is at address 2*i + 1 and the right one is at 2*i+2,
  // because of that we can just add the value for direction to avoid a conditional
  return 2 * parent_id + 1 + static_cast<size_t> (type);
}

BinaryTree::NodeId BinaryTree::RootNodeId() {
  return 0;
}

void BinaryTree::SetNode(BinaryTree::NodeId node_id, BinaryTree::T value) {
  nodes_[node_id] = value;
}

// WARN: That is not tested
void BinaryTree::SetNode(BinaryTree::NodeId node_id,
                         const BinaryTree &tree,
                         NodeId root_node = BinaryTree::RootNodeId()) {
  SetNode(node_id, tree.GetNodeValue(root_node));
  NodeId current_node_source = root_node;
  NodeId current_node_target = node_id;

  auto[source_left, source_right] = tree.GetChildrenIds(current_node_source);
  auto[target_left, target_right] = GetChildrenIds(current_node_target);

  // FIXME: we are double assigning here!! Need to take care of it l8er!!

  if (auto left_value = tree.GetNodeValue(source_left)) {
    InsertChild(target_left, BinaryTree::ChildType::LEFT, left_value);
    SetNode(target_left, tree, source_left);
  }

  if (auto right_value = tree.GetNodeValue(source_left)) {
    InsertChild(target_right, BinaryTree::ChildType::RIGHT, right_value);
    SetNode(source_right, tree, source_right);
  }
}

BinaryTree::NodeId BinaryTree::InsertTree(BinaryTree::NodeId parent_id,
                                          BinaryTree::ChildType type,
                                          const BinaryTree &tree) {
  NodeId target_node = GetChild(parent_id, type);
  SetNode(target_node, tree);
  return target_node;
}
std::vector<BinaryTree::NodeId> BinaryTree::NodeIterator() {
  assert(false);
  std::vector<NodeId> existing_nodes;

  for (int i = 0; i < nodes_.size(); i++) {
    if (nodes_[i].has_value()) {
      existing_nodes.push_back(i);
    }
  }
  return existing_nodes;
}

bool BinaryTree::HasValue(BinaryTree::NodeId node_id) const {
  return GetNodeValue(node_id).has_value();
}

bool BinaryTree::Exists(BinaryTree::NodeId node_id) {
  return node_id < nodes_.size();
}
