//
// Created by dims_ on 13/10/2021.
//

#include "Harness.h"
#include "compression.h"
#include "src/lzw/lzw.h"
#include <iostream>

int main() {
  HarnessConfig config;

  // There is an infinite loop if file is to small

  config.text_lengths = {20, 100, 1 KB, 2 KB, 100 KB, 116 KB};
  config.number_of_repetitions = 100;

  // WARN: Not cleaning up the executors
  Harness h(config, {{"RLE", new Rle}, {"LZW", new Lzw}}, {"./makbet.txt", "./lena.jpg"});

  h.Run();

  auto results = h.GetResults();

  for (const auto &r: results) { std::cout << r << std::endl << std::endl; }

  return 0;
}
