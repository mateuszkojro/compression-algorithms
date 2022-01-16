//
// Created by dims_ on 13/10/2021.
//

#include "BitVector.h"
#include <cassert>
namespace mk {

/// \warning Bits are interpreted in little endian
std::string BitVector::ToAsciiString() const {
  std::string chars;
  chars.reserve(1 + Size() / (chunk_size_ / 8));

  int char_size = sizeof(char) * 8;
  for (size_t i = 0; i < Size() / char_size; i += char_size) {
    char c = 0;
    // Loop over the bits of one char
    for (size_t j = 0; j < char_size; j++) {
      c += static_cast<char>(
          GetBit(i + j) // We are creating a character here NOLINT(cppcoreguidelines-narrowing-conversions)
              << j);
    }
    chars.push_back(c);
  }
  return chars;
}

bool BitVector::GetBit(size_t index) const {
  size_t chunk_index = index / chunk_size_;
  size_t inner_index = index % chunk_size_;
  return bits_[chunk_index][inner_index];
}

void BitVector::Set(size_t index, bool value) {
  size_t chunk_index = index / chunk_size_;
  size_t inner_index = index % chunk_size_;
  bits_[chunk_index][inner_index] = value;
}

size_t BitVector::Size() const {
  return size_;
}

void BitVector::SetSizeInternal(size_t size) {
  size_ = size;
  bits_.reserve(1 + size / chunk_size_);
  for (size_t i = 0; i < ((Size() / chunk_size_) + 1); i++) {
    bits_.emplace_back();
  }
}
BitVector::BitVector() = default;

BitVector::BitVector(size_t size) {
  SetSizeInternal(size);
}
BitVector::BitVector(const std::string &text) {
  SetSizeInternal(text.size() / sizeof(char) + text.size() % sizeof(char));

  for (size_t j = 0; j < text.size(); j += 8) {
    for (size_t i = 0; i < 8; i++) {
      Set(i + j, text[j]);
    }
  }
}
void BitVector::PushBack(bool value) {
  size_t current_size = size_;
  if (Size() < current_size) {
    Set(Size(), value);
  } else {
    this->bits_.emplace_back();
    Set(Size(), value);
  }
  size_++;
}
//void BitVector::Reserve(int) {
//  throw NotImplemented();
//}

}

