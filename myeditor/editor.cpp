/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#include "myeditor/editor.h"

#include "myeditor/log.h"
#include "myeditor/mod_manager.h"
#include "myeditor/panel_context_manager.h"

namespace myeditor {

Editor::Editor() 
  : mod_manager_(new ModManager())
  , panel_context_manager_(new PanelContextManager()) {
  rate_ctrl_.Reset();
}

Editor::~Editor() {
  state_ = State::kUninitialized;
}

bool Editor::Init() {
  // TODO load panel dir conf
  // TODO load lib
  // TODO load panel inst
  state_ = State::kInitialized;
  return true;
}

void Editor::Update() {
  panel_context_manager_->Update();
}

void Editor::ProcMessage() {
  panel_context_manager_->ProcMessage();
}

void Editor::Render() {
  panel_context_manager_->Render();
}

bool Editor::Sleep() {
  return rate_ctrl_.Sleep();
}

}  // namespace myeditor
