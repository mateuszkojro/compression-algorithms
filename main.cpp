//
// Created by M.Kojro on 13/10/2021.
//

#include "Harness.h"
#include "compression.h"
#include "src/lzw/lzw.h"
#include <iostream>

int main() {
  HarnessConfig config;

  config.text_lengths = {20, 100, 1 KB, 2 KB, 100 KB, 116 KB};
  config.number_of_repetitions = 100;

  Harness harness
      (config, {{"RLE", new Rle},
                {"Huffman", new HuffmanCoding},
                {"LZW", new Lzw}},
       {"./makbet.txt", "./lena.jpg"});

  harness.Run();

  auto results = harness.GetResults();

  for (const auto &record: results) { std::cout << record << std::endl << std::endl; }

  harness.ToCSV(std::cout);

  return 0;
}
