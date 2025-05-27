/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#include "myeditor/panel_context.h"

#include "myeditor/panel.h"

namespace myeditor {

bool PanelContext::Init() {
  // TODO init config
  return panel_->Init();
}

void PanelContext::Update() {
  if (show_) {
    panel_->Update();
  }
}

void PanelContext::ProcMessage() {
  for (auto it : recv_) {
    panel_->Proc(it);
  }
  recv_.clear();
}

void PanelContext::RecvMessage(std::shared_ptr<myframe::Msg> msg) {
  recv_.emplace_back(std::move(msg));
}

void PanelContext::Render() {
  panel_->Render();
}

std::string PanelContext::GetPanelName() const {
  return "panel." + class_name_ + "." + instance_name_; 
}

bool PanelContext::IsShow() const {
  return show_;
}

void PanelContext::SetShow(bool b) {
  show_ = b;
}

bool PanelContext::IsDisplayMenu() const {
  return display_menu_;
}

void PanelContext::SetDisplayMenu(bool b) {
  display_menu_ = b;
}

}  // namespace myeditor
