//
// Created by piotr on 18/10/2021.
//
#include "pm_include.h"
int main(){
  {
    START_TRACER("first");
//    std::couta << "hello pm";
  }
  SAVE_TRACINGS("sommy");

  return 0;
}