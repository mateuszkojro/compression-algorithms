//
// Created by piotr on 16/10/2021.
//

#ifndef COA_CLOCK_TRACER_H_
#define COA_CLOCK_TRACER_H_

#include <stack>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#define DEBUG_MODE true
#define AUTO_FUNCTION_NAMES false

#if DEBUG_MODE
#if AUTO_FUNCTION_NAMES
#define START_TRACER Tracer tracer(__FILE__, __func__)
#else
#define START_TRACER(x) Tracer tracer(x)
#endif
#define SAVE_TRACINGS(x) Tracer::SaveToJSON(x)
#else

#define START_TRACER(x)
#define SAVE_TRACINGS(x)
#endif
enum class Type {
  BEGIN,
  END
};
struct Tracers {
  Tracers(std::string name, Type type, long long int time_point, size_t thread_id);

  std::string name;
  Type type;
  long long int time_point;
  size_t thread_id;
  std::string Save();
};

class Tracer {
 public:
  Tracer() = delete;
  Tracer(std::string clock_name);
  Tracer(const std::string &file_name, const std::string &function_name);
  Tracer(const Tracer &other) = delete;
  Tracer &operator=(const Tracer &other) = delete;

  ~Tracer();
  static void SaveToJSON(const std::string &filename);

  static std::vector<Tracers> saved_tracers_;

 protected:
  std::string tracer_name_;
  std::chrono::time_point<std::chrono::high_resolution_clock> time_point_;
  size_t thread_id_ = std::hash<std::thread::id>{}(std::this_thread::get_id());
};

#endif//COA_CLOCK_TRACER_H_
