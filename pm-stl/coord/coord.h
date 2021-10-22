//
// Created by studio25 on 29.07.2021.
//
/// describes point in 2 d space
#ifndef BLOCK_V2_UTILITY_COORD_H_
#define BLOCK_V2_UTILITY_COORD_H_


#include <cassert>
#include <ciso646>
#include <ostream>

struct Coord {

  Coord();
  Coord(int x, int y);
  Coord(const Coord &other);

  Coord &operator=(const Coord &other);

  bool operator==(const Coord &rhs) const;
  bool operator!=(const Coord &rhs) const;
  /// output to console, for debug purposes
  friend std::ostream &operator<<(std::ostream &os, const Coord &coord);

  /// 2c point to 1d conversion
  unsigned ToInt(unsigned width) const {
    assert(x < width);
    return y * width + x;
  }
  bool operator<(const Coord &rhs) const;
  bool operator>(const Coord &rhs) const;
  bool operator<=(const Coord &rhs) const;
  bool operator>=(const Coord &rhs) const;

  int x;
  int y;
};

#endif // BLOCK_V2_UTILITY_COORD_H_
