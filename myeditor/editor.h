/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include <string>
#include <memory>
#include <atomic>
#include <json/json.h>

#include "myeditor/export.h"
#include "myeditor/rate_ctrl.h"

namespace myeditor {

struct MYEDITOR_EXPORT EditorConfig {
  EditorConfig();

  std::string lib_dir;
};

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

  bool Init(const EditorConfig& config);

  bool LoadPanel(const Json::Value& config);

  void Update();

  void ProcMessage();

  void Render();

  bool Sleep();

  PanelContextManager* GetPanelContextManager() const;

 private:
  std::atomic<State> state_{State::kUninitialized};
  RateCtrl rate_ctrl_;

  std::unique_ptr<ModManager> mod_manager_;

  std::unique_ptr<PanelContextManager> panel_context_manager_;
};

}  // namespace myeditor