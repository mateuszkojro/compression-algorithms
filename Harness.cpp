//
// Created by M.Kojro on 11/27/2021.
//

#include "Harness.h"
#include <cmath>
#include <fstream>
#include <ios>
#include <iostream>

size_t FindDelta(const std::string& s_1, const std::string& s_2) {

  int64_t size_d = s_1.size() - s_2.size();
  if (size_d != 0) {
	return s_1.size() - size_d;
  }

  for (size_t i = 0; i < s_1.size(); i++) {
	if (s_1[i] != s_2[1]) {
	  return i;
	}
  }
  return 0;
}

size_t bit_size(const std::string& string) { return string.size() * 8; }

template<class T>
double Avg(const std::vector<T>& data) {
  double sum = std::accumulate(data.begin(), data.end(), 0.0);
  return sum / static_cast<double>(data.size());
}

template<class T>
double StdDeviation(const std::vector<T>& data, double avg = 0) {
  if (avg == 0) {
	avg = Avg(data);
  }
  double sum = 0;
  for (auto& elem : data) { sum += pow(static_cast<double>(elem) - avg, 2.0); }
  return sqrt(sum / static_cast<double>(data.size()));
}

// FIXME: could just read specified size in one go straight from file
std::string ReadFile(std::fstream& fd) {
  std::string whole_file;
  std::string line;

  assert(fd.good() && "Passed bad file descriptor");

  while (fd.good()) {
	std::getline(fd, line);
	// assert(fd.good() && "Reading line failed");
	whole_file.append(line);
  }
  return whole_file;
}

void Harness::Run() {
  for (auto& dataset : datasets_) {

	std::fstream file(dataset, std::ios::in | std::ios::binary);

	std::string dataset_text = ReadFile(file);
	RunOverDataset(dataset, dataset_text, config_.number_of_repetitions);
  }
}

const std::vector<BenchmarkRecord>& Harness::GetResults() { return results_; }

std::ostream& operator<<(std::ostream& os, const BenchmarkRecord& record) {
  os << "success: " << std::boolalpha << record.success
	 << " executor_name: " << record.executor_name
	 << " dataset_name: " << record.dataset_name
	 << " text_length: " << record.text_length
	 << " avg_compression_time: " << record.avg_compression_time
	 << " std_dev_compression_time: " << record.std_dev_compression_time
	 << " avg_decompression_time: " << record.avg_decompression_time
	 << " std_dev_decompression_time: " << record.std_dev_decompression_time
	 << " compression_ratio: " << record.compression_ratio;
  return os;
}

Harness::~Harness() {
  for (auto& [name, value] : executors_) { delete value; }
}

// TODO(mateusz): Could be multi threaded but need to take care of the executor
// for each thread
void Harness::RunOverDataset(
	const std::string&
		dataset_name,// NOLINT(bugprone-easily-swappable-parameters)
	const std::string& dataset_text, size_t times) {

  for (auto length : config_.text_lengths) {

	for (auto& [name, executor] : executors_) {

	  bool success = true;
	  double compression_ratio = 0;

	  std::vector<double> compression_times;
	  std::vector<double> decompression_times;
	  compression_times.reserve(times);
	  decompression_times.reserve(times);

	  for (size_t r = 0; r < times; r++) {
		std::clog << r << "Running: " << dataset_name
				  << " with length: " << length << " and executor: " << name
				  << std::endl;

		auto text = dataset_text.substr(0, length);

		auto [compress_time] = executor->Compress(text);
		compression_times.push_back(
			compress_time.count());// WARN: Narrowing from long long to double
								   // might lose precision

		auto [decompress_time, decompressed_text] = executor->Decompress();
		decompression_times.push_back(decompress_time.count());

		compression_ratio = static_cast<double>(executor->CompressedSize())
							/ static_cast<double>(bit_size(text));

		success = decompressed_text == text;
		if (!success) {
		  std::cout << "ERROR:  " << name << " did not compress "
					<< dataset_name << " correctly, delta at: "
					<< FindDelta(text, decompressed_text) << std::endl;

		  assert(false && "Text after compression is not the same as original");
		}
	  }

	  auto compression_time_avg = Avg(compression_times);
	  auto std_dev_compression_time =
		  StdDeviation(compression_times, compression_time_avg);

	  auto decompression_time_avg = Avg(decompression_times);
	  auto std_dev_decompression_time =
		  StdDeviation(decompression_times, decompression_time_avg);

	  //   std::lock_guard<std::mutex> lock(results_mutex_);
	  results_.push_back({success, name, dataset_name, length,
						  compression_time_avg, std_dev_compression_time,
						  decompression_time_avg, std_dev_decompression_time,
						  compression_ratio});
	}
  }
}
