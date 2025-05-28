/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include <imgui.h>

#include "myeditor/panel.h"

namespace myeditor {

class PanelLog : public Panel {
 public:
  virtual ~PanelLog();

  bool Init() override;

  void Update() override;

 private:
  void Clear();
  void AddLog(const char* fmt, ...) IM_FMTARGS(2);
  void AddLogCB(const std::string& msg);

  ImGuiTextBuffer Buf;
  ImGuiTextFilter Filter;
  ImVector<int>
      LineOffsets;  // Index to lines offset. We maintain this with AddLog()
                    // calls, allowing us to have a random access on lines
  bool AutoScroll = true;  // Keep scrolling if already at the bottom
};

}  // namespace myeditor
