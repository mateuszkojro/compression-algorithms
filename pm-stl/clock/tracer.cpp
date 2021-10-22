//
// Created by piotr on 16/10/2021.
//

#include "tracer.h"
#include <fstream>
#include <utility>

std::chrono::time_point<std::chrono::high_resolution_clock> global_begin_ = std::chrono::high_resolution_clock::now();
std::vector<Tracers> Tracer::saved_tracers_;
std::string GenFunctionName2(const std::string &file_name, const std::string &function_name) {
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

Tracer::Tracer(std::string clock_name) : tracer_name_(std::move(clock_name)) {

  saved_tracers_.emplace_back(tracer_name_, Type::BEGIN, std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - global_begin_).count(), thread_id_);
}
Tracer::Tracer(const std::string &file_name, const std::string &function_name) : tracer_name_(GenFunctionName2(file_name, function_name)) {

  saved_tracers_.emplace_back(tracer_name_, Type::BEGIN, std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - global_begin_).count(), thread_id_);
}
Tracer::~Tracer() {
  saved_tracers_.emplace_back(tracer_name_, Type::END, std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - global_begin_).count(), thread_id_);
}
void Tracer::SaveToJSON(const std::string &filename) {

  std::fstream output_file;
  output_file.open( filename + ".JSON", std::ios::out);
  output_file << "{\"traceEvents\": [";
  //  for (auto t : saved_tracers_)
  //    output_file << t.Save();
  //
  for (int i = 0; i < saved_tracers_.size(); ++i) {
    output_file << saved_tracers_[i].Save();
    if (i < saved_tracers_.size() - 1)
      output_file << ',';
  }

  output_file << "   ],\n"
                 "    \"displayTimeUnit\": \"ns\",\n"
                 "    \"systemTraceEvents\": \"SystemTraceData\",\n"
                 "    \"otherData\": {\n"
                 "        \"version\": \"Unknown Application\"\n"
                 "    }\n"
                 "}";

  output_file.close();
}
std::string Tracers::Save() {

  std::string buffer;
  buffer += " {\"name\":";
  buffer += "\"" + name + "\"";
  buffer += ",";
  buffer += "\"cat\":\"IDK\",";
  buffer += "\"ph\":";
  buffer += type == Type::BEGIN ? "\"B\"" : "\"E\"";
  buffer += ",";
  buffer += "\"pid\":\"1\",";
  buffer += "\"tid\":";
  buffer += "\"Thread Id: " + std::to_string(thread_id) + "\"";
  buffer += ",";
  buffer += "\"ts\":";
  buffer += "\"" + std::to_string(time_point) + "\"";
  buffer += "}";
  return buffer;
}
Tracers::Tracers(std::string name, Type type, long long int time_point, size_t thread_id) : name(std::move(name)), type(type), time_point(time_point), thread_id(thread_id) {}
