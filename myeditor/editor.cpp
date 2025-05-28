/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#include "myeditor/editor.h"

#include "myeditor/log.h"
#include "myeditor/mod_manager.h"
#include "myeditor/panel_context_manager.h"
#include "myeditor/builtin/panel_log.h"
#include "myeditor/panel_context.h"

namespace myeditor {

EditorConfig::EditorConfig() {
  lib_dir = "lib";
}

Editor::Editor() {
  rate_ctrl_.Reset();
}

Editor::~Editor() {
  state_.store(State::kUninitialized);
}

bool Editor::LoadPanel(const Json::Value& config) {
  if (state_.load() == State::kUninitialized) {
    return false;
  }
  // load panel context
  std::vector<std::shared_ptr<PanelContext>> panel_ctx_list;
  if (!mod_manager_->CreatePanelContext(config, &panel_ctx_list)) {
    LOG(ERROR) << "Create panel context failed";
    return false;
  }
  // add to manager
  for (size_t i = 0; i < panel_ctx_list.size(); ++i) {
    auto panel_ctx = panel_ctx_list[i];
    panel_ctx->SetPanelContextManger(panel_context_manager_.get());
    LOG(INFO) << "create panel " << panel_ctx->GetPanelName();
    panel_context_manager_->RegPanelContext(panel_ctx);
  }
  return true;
}

bool Editor::Init(const EditorConfig& config) {
  if (state_.load() == State::kInitialized) {
    return true;
  }
  auto lib_dir = myframe::Common::GetAbsolutePath(config.lib_dir);
  mod_manager_ = std::make_unique<ModManager>(lib_dir.string());
  panel_context_manager_ = std::make_unique<PanelContextManager>();

  // reg builtin panel
  mod_manager_->RegPanel("PanelLog", [](const std::string&){
    return std::make_shared<PanelLog>();
  });

  state_.store(State::kInitialized);
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

PanelContextManager* Editor::GetPanelContextManager() const {
  return panel_context_manager_.get();
}

}  // namespace myeditor
