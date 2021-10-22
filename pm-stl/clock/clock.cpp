//
// Created by piotr on 15/10/2021.
//

#include "clock.h"
#include <fstream>
std::map<std::string, long long unsigned int> Clock::timings_;
std::stack<Clock *> Clock::clocks_;

std::string GenFunctionName(const std::string &file_name, const std::string &function_name) {
  std::string local_filename = file_name;

  int dot_placement = 0;
  for (int i = 0; i < local_filename.size(); i++) {
    if (local_filename[i] == '.') {
      dot_placement = i;
      break;
    }
  }
  if (local_filename[dot_placement + 1] == 'c' && local_filename[dot_placement + 2] == 'p' && local_filename[dot_placement + 3] == 'p')
    local_filename.erase(dot_placement, dot_placement + 3);
  else if (local_filename[dot_placement + 1] == 'c' && local_filename[dot_placement + 2] != 'p') {
    local_filename.erase(dot_placement, dot_placement + 2);
  }
  local_filename.push_back('/');
  int slash_placement = 0;
  for (int i = dot_placement - 1; i >= 0; --i) {
    if (local_filename[i] == '\\') {
      slash_placement = i;
      break;
    }
  }
  local_filename.erase(0, slash_placement + 1);
  return local_filename + function_name;
}

Clock::Clock(const std::string &file_name, const std::string &function_name) : clock_name_(GenFunctionName(file_name, function_name)) {

  if (!clocks_.empty()) clocks_.top()->Stop();
  clocks_.push(this);
  Start();
}
Clock::Clock(int line) : clock_name_(std::to_string(line)) {
  if (!clocks_.empty()) clocks_.top()->Stop();
  clocks_.push(this);
  Start();
}

Clock::Clock(std::string clock_name) : clock_name_(std::move(clock_name)) {
  if (!clocks_.empty()) clocks_.top()->Stop();
  clocks_.push(this);
  Start();
}

Clock::~Clock() {
  Stop();
  clocks_.pop();
  if (!clocks_.empty()) clocks_.top()->Start();
}
void Clock::SaveToFile(const std::string &filename) {
  std::fstream output_file;
  output_file.open("../testing/" + filename, std::ios::out);
  //todo rewrite this loop correctly
  int i = 0;
  for (auto t = timings_.begin(); t != timings_.end(); ++t) {
    output_file << t->first << '\t' << t->second;
    // the file can't have '\n' or ' ' or '\t' symbol at the end,
    if (++i != timings_.size())
      output_file << '\n';
  }
  output_file.close();
}

void Clock::Stop() {
  auto time_point_2 = std::chrono::high_resolution_clock::now();
  if (timings_.find(clock_name_) != timings_.end())
    timings_[clock_name_] += std::chrono::duration_cast<std::chrono::microseconds>(time_point_2 - time_point_).count();
  else
    timings_[clock_name_] = std::chrono::duration_cast<std::chrono::microseconds>(time_point_2 - time_point_).count();
}
void Clock::Start() {
  time_point_ = std::chrono::high_resolution_clock::now();
}
