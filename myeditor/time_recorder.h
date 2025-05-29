/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include <chrono>
#include <ratio>
#include <string>
#include <vector>

namespace myeditor {

using stdclock = std::chrono::steady_clock;

struct TimeInfo {
  int step;
  std::string desc;
  stdclock::time_point ts;

  TimeInfo(int s, const std::string& d);
};

class TimeRecorder {
 public:
  static TimeRecorder* Instance();

  void Record(const std::string& desc);

  void Clear();

  std::string TimeCostInfo();

 private:
  TimeRecorder() {}
  ~TimeRecorder(){};
  TimeRecorder(const TimeRecorder&);
  TimeRecorder& operator=(const TimeRecorder&);

 private:
  int _idx = 0;
  std::vector<TimeInfo> _tss;
};

}  // namespace myeditor
