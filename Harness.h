//
// Created by M.Kojro on 11/27/2021.
//

#ifndef TEST_APPLICATION__HARNESS_H_
#define TEST_APPLICATION__HARNESS_H_

#include <cassert>
#include <iostream>
#include <mutex>
#include <numeric>
#include <thread>
#include <utility>
#include <vector>

#include "src/Compressor.h"

template<class T>
struct NamedObject
{
  std::string name;
  T value;
};

#define KB *1024lu
#define MB *1024lu * 1024lu

struct HarnessConfig
{
  size_t number_of_repetitions = 10;
  std::vector<size_t> text_lengths = {1 KB, 1 MB};
};

struct BenchmarkRecord
{
  friend std::ostream& operator<<(std::ostream& os,
								  const BenchmarkRecord& record);
  bool success;
  std::string executor_name;
  std::string dataset_name;
  size_t text_length;
  double avg_compression_time;
  double std_dev_compression_time;
  double avg_decompression_time;
  double std_dev_decompression_time;
  double compression_ratio;
};

using T = Compressor*;
class Harness {
 public:
  Harness(HarnessConfig config, std::vector<NamedObject<T>> executors,
		  std::vector<std::string> datasets)
	  : executors_(std::move(executors)), datasets_(std::move(datasets)),
		config_(std::move(config)) {}
  ~Harness();
  void Run();
  const std::vector<BenchmarkRecord>& GetResults();
  std::ostream & ToCSV(std::ostream& ostream);

 private:
  void RunOverDataset(const std::string& dataset_name,
					  const std::string& dataset_text, size_t times);

  std::mutex results_mutex_;
  HarnessConfig config_;
  std::vector<BenchmarkRecord> results_;
  std::vector<NamedObject<T>> executors_;
  std::vector<std::string> datasets_;
};

#endif// TEST_APPLICATION__HARNESS_H_
