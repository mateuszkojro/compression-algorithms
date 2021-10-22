//
// Created by dims_ on 13/10/2021.
//

#ifndef TEST_APPLICATION_SRC_BIT_VECTOR_BITVECTOR_H_
#define TEST_APPLICATION_SRC_BIT_VECTOR_BITVECTOR_H_

#include <vector>
#include <bitset>

#define MK_BITVECTOR_CHUNK_SIZE 64

/// Space optimized variable length bit vector
namespace mk {
class BitVector {
 public:
  explicit BitVector();
  explicit BitVector(size_t size);
  explicit BitVector(const std::string &text);

  [[nodiscard]] bool Get(size_t index) const;
  [[nodiscard]] size_t Size() const;
  [[nodiscard]] std::string ToAsciiString() const;

  void Set(size_t index, bool value);
  void PushBack(bool value);

 private:
  void SetSizeInternal(size_t size);
  const int chunk_size_ = MK_BITVECTOR_CHUNK_SIZE;
  size_t size_ = 0;
  std::vector<std::bitset<MK_BITVECTOR_CHUNK_SIZE>> bits_;
};
}

#endif //TEST_APPLICATION_SRC_BIT_VECTOR_BITVECTOR_H_
