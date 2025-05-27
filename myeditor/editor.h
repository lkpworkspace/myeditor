/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include <memory>

#include "myeditor/export.h"
#include "myeditor/rate_ctrl.h"

namespace myeditor {

class ModManager;
class PanelContextManager;
class MYEDITOR_EXPORT Editor final {
 public:
  enum class State : std::uint8_t {
    kUninitialized = 0,
    kInitialized
  };

  Editor();
  virtual ~Editor();

  bool Init();

  void Update();

  void ProcMessage();

  void Render();

  bool Sleep();

 private:
  State state_{State::kUninitialized};
  RateCtrl rate_ctrl_;

  std::unique_ptr<ModManager> mod_manager_;

  std::unique_ptr<PanelContextManager> panel_context_manager_;
};

}  // namespace myeditor