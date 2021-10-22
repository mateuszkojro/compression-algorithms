//
// Created by dims_ on 13/10/2021.
//

#include <iostream>
#include "compression.h"
#include "pm_include.h"
int main() {
  {
  START_TRACER("x");
  srand(10);
  }
  mk::BitVector vector(10);
  for (int i = 0; i < vector.Size(); i++) {
    vector.Set(i, rand() % 2); // NOLINT(cert-msc50-cpp)
  }

  for (int i = 0; i < vector.Size(); i++) {
    std::cout << i << " => " << vector.Get(i) << std::endl;
  }

  for (int i = 0; i < 10; i++) {
    vector.PushBack(true); // NOLINT(cert-msc50-cpp)
  }

  std::cout << std::endl << "After 10 pushbacks" << std::endl;

  for (int i = 0; i < vector.Size(); i++) {
    std::cout << i << " => " << vector.Get(i) << std::endl;
  }

  mk::BitVector b(8);
  int i = 0;
  b.Set(i++, false); // Most significant
  b.Set(i++, true);
  b.Set(i++, false);
  b.Set(i++, false);
  b.Set(i++, false);
  b.Set(i++, false);
  b.Set(i++, true);

  std::cout << "B: " << b.ToAsciiString() << std::endl;
  SAVE_TRACINGS("xddd");
  return 0;
}