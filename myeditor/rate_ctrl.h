/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include <chrono>
#include <thread>

#include "myeditor/export.h"

namespace myeditor {

using stdclock = std::chrono::steady_clock;

class MYEDITOR_EXPORT RateCtrl {
 public:
  RateCtrl(int frequency = 30);

  bool Sleep();

  void Reset();

 private:
  stdclock::time_point start_;
  int expected_cycle_time_;
};

}  // namespace myeditor
