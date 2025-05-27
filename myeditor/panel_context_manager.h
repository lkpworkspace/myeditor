/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include <memory>
#include <string>
#include <unordered_map>

namespace myeditor {

class PanelContext;
class PanelContextManager {
 public:
  bool RegPanelContext(std::shared_ptr<PanelContext> pc);

  std::shared_ptr<PanelContext> GetPanelContext(const std::string& panel_name);

  void Update();

  void ProcMessage();

  void Render();

 private:
  // key: panel.classname.inst
  // value: panel context obj
  std::unordered_map<std::string, std::shared_ptr<PanelContext>> panel_ctxs_;
};

}  // namespace myeditor
