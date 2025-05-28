/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#include "myeditor/panel.h"

#include "myeditor/panel_context.h"
#include "myeditor/panel_context_manager.h"

namespace myeditor {

void Panel::Send(const std::string& dst, std::shared_ptr<myframe::Msg> msg) {
  msg->SetSrc(GetPanelName());
  msg->SetDst(dst);
  auto pcm = ctx_->GetPanelContextManger();
  pcm->DispatchMessage(std::move(msg));
}

std::string Panel::GetPanelName() const {
  return ctx_->GetPanelName();
}

bool Panel::IsShow() const {
  return ctx_->IsShow();
}

void Panel::SetPanelContext(PanelContext* pc) {
  ctx_ = pc;
}

} // namespace myeditor
