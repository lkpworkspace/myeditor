/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include <imgui.h>

#include "myeditor/panel.h"

namespace myeditor {

class PanelMenu : public Panel {
 public:
  virtual ~PanelMenu();

  bool Init() override;

  void Update() override;
};

}  // namespace myeditor
