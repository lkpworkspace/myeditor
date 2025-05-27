/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#include "myeditor/rate_ctrl.h"

namespace myeditor {

RateCtrl::RateCtrl(int frequency)
  : start_(stdclock::now())
  , expected_cycle_time_(int(1000.0 / frequency))
{}

bool RateCtrl::Sleep() {
  auto expected_end = start_ + std::chrono::milliseconds(expected_cycle_time_);
  auto actual_end = stdclock::now();
  auto sleep_time = std::chrono::duration_cast<std::chrono::milliseconds>(
    expected_end - actual_end).count();

  start_ = expected_end;

  if (sleep_time < 0) {
    if (actual_end >
        (expected_end + std::chrono::milliseconds(expected_cycle_time_))) {
      start_ = actual_end;
    }
    return false;
  } else {
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
  }
  return true;
}

void RateCtrl::Reset() { start_ = stdclock::now(); }

}  // namespace myeditor
