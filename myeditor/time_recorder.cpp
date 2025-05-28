/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#include "myeditor/time_recorder.h"

#include <sstream>

namespace myeditor {

TimeInfo::TimeInfo(int s, const std::string& d)
    : step(s), desc(d), ts(stdclock::now()) {}

TimeRecorder* TimeRecorder::Instance() {
  static TimeRecorder tr;
  return &tr;
}

void TimeRecorder::Record(const std::string& desc) {
  _tss.emplace_back(++_idx, desc);
}

void TimeRecorder::Clear() {
  _idx = 0;
  _tss.clear();
}

std::string TimeRecorder::TimeCostInfo() {
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

}  // namespace myeditor
