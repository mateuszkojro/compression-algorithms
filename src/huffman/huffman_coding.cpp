#include "huffman_coding.h"
#include <queue>
#include <map>
#include <cassert>

template<class Type>
struct FreqAndValue {
  FreqAndValue(uint64_t freq, Type value) : freq(freq), value(value) {}
  uint64_t freq;
  Type value;
};

using FreqType = FreqAndValue<char>;
using PriorityQueue = std::priority_queue<FreqType>;

namespace std {
template<>
struct less<FreqType> {
  bool operator()(const FreqType &a, const FreqType &b) const {
    return a.freq > b.freq;
  }
};
}

PriorityQueue sort_by_freqs(const std::string &values) {
  std::unordered_map<uint64_t, char> freq_map;
  for (auto letter: values) {
    freq_map[letter]++;
  }

  std::priority_queue<FreqType> sorted_que;
  for (auto&[character, frequency]: freq_map) {
    sorted_que.emplace(frequency, character);
  }

  return sorted_que;
}

template<class T>
T pop(std::queue<T> &queue) {
  auto elem = queue.front();
  queue.pop();
  return elem;
}

template<class T>
std::vector<bool> join(const std::vector<T> a, const std::vector<T> b) {
  std::vector<T> result = a;
  result.reserve(a.size() + b.size());
  for (auto elem: b) {
    result.push_back(elem);
  }
  return result;
}

std::optional<std::vector<bool>> find_char(char chacracter, BinaryTreeNode *node) {
  if (!node) {
    return std::nullopt;
  }

  if (node->value.has_value() && node->value.value() == chacracter) {
    return std::vector<bool>();
  }

  auto left = find_char(chacracter, node->left);
  if (left.has_value()) {
    auto value = left.value();
    value.push_back(0);
    return value;
  }

  auto right = find_char(chacracter, node->left);
  if (right.has_value()) {
    auto value = right.value();
    value.push_back(1);
    return value;
  }

  return std::nullopt;
}

TimeAndValue<void> HuffmanCoding::Compress(const std::string &text) {
  auto start = std::chrono::high_resolution_clock::now();
  std::queue<BinaryTreeNode *> nodes;
  auto sorted_by_freq = sort_by_freqs(text);
  while (!sorted_by_freq.empty()) {
    auto elem = sorted_by_freq.top();
    sorted_by_freq.pop();
    nodes.push(new BinaryTreeNode(elem.value, elem.freq));
  }

  while (nodes.size() != 1) {
    auto *node1 = pop(nodes);
    auto *node2 = pop(nodes);

    nodes.push(new BinaryTreeNode(node1->freq + node2->freq, node1, node2));
  }
  compressed_.reserve(text.size() * 8);
  for (auto letter: text) {
    auto maybe_code = find_char(letter, nodes.front());
    assert(maybe_code.has_value());
    compressed_ = join(compressed_, maybe_code.value());
  }

  base_node_ = nodes;

  auto stop = std::chrono::high_resolution_clock::now();
  return TimeAndValue<void>(std::chrono::duration_cast<std::chrono::microseconds>(stop - start));
}

std::optional<char> decode_char(std::vector<bool> &encoded, BinaryTreeNode *base_node) {
  auto *current_node = base_node;
  for (size_t i = 0; i < encoded.size(); i++) {
    if (encoded[i] == 0) {
      current_node = current_node->left;
    } else {
      current_node = current_node->right;
    }
    encoded.erase(encoded.begin());
    if (current_node->value.has_value()) {
      return current_node->value.value();
    }
  }

  return std::nullopt;
}

TimeAndValue<std::string> HuffmanCoding::Decompress() {
  auto start = std::chrono::high_resolution_clock::now();
  std::string decompressed;
  std::optional<char> result = std::nullopt;
  do {
    result = decode_char(compressed_, base_node_.front());
    assert(result.has_value());
    decompressed.push_back(result.value());
  } while (result.has_value());
  auto stop = std::chrono::high_resolution_clock::now();
  return TimeAndValue<std::string>(decompressed, std::chrono::duration_cast<std::chrono::microseconds>(stop - start));
}

size_t HuffmanCoding::CompressedSize() {
  return compressed_.size() + base_node_.front()->size();
}
