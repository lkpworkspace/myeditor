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

  void Record(const std::string& desc) { _tss.emplace_back(++_idx, desc); }

  void Clear() {
    _idx = 0;
    _tss.clear();
  }

  std::string TimeCostInfo() {
    if (_tss.empty()) {
      return "";
    }
    std::stringstream ss;
    ss << "time cost info:\n" << _tss[0].desc << "\n";
    for (int i = 0; i < _tss.size() - 1; ++i) {
      auto cost_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                         _tss[i + 1].ts - _tss[i].ts)
                         .count();
      ss << _tss[i + 1].desc << " cost " << cost_ms << "ms\n";
    }
    return ss.str();
  }

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
