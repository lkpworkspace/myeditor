/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#include "myeditor/panel_context_manager.h"

#include "myeditor/log.h"
#include "myeditor/panel_context.h"

namespace myeditor {

bool PanelContextManager::RegPanelContext(
  std::shared_ptr<PanelContext> pc) {
  auto panel_ctx_it = panel_ctxs_.find(pc->GetPanelName());
  if (panel_ctx_it == panel_ctxs_.end()) {
    if (!pc->Init()) {
      LOG(ERROR) << "Reg panel context " << pc->GetPanelName() << " failed";
      return false;
    }
    panel_ctxs_[pc->GetPanelName()] = pc;
    return true;
  }
  return false;
}

std::shared_ptr<PanelContext> PanelContextManager::GetPanelContext(
  const std::string& panel_name) {
  auto panel_ctx_it = panel_ctxs_.find(panel_name);
  if (panel_ctx_it == panel_ctxs_.end()) {
    return nullptr;
  }
  return panel_ctx_it->second;
}

void PanelContextManager::Update() {
  for (auto it : panel_ctxs_) {
    (it.second)->Update();
  }
}

void PanelContextManager::ProcMessage() {
  for (auto it : panel_ctxs_) {
    (it.second)->ProcMessage();
  }
}

void PanelContextManager::Render() {
  for (auto it : panel_ctxs_) {
    (it.second)->Render();
  }
}

} // namespace myeditor
