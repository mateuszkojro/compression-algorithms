//
// Created by piotr on 15/10/2021.
//
#include "clock.h"
#include "tracer.h"
#include <iostream>
#include <thread>

int Fun2();

int Fun() {
  START_TRACER;
  std::this_thread::sleep_for(std::chrono::seconds(1));

  return Fun2();
}

int Fun2() {
  START_TRACER;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  return 7;
}

int main() {

  Fun();
  SAVE_TRACINGS("test_son");
  return 9;
}