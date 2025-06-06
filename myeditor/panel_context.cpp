/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#include "myeditor/panel_context.h"

#include "myeditor/panel.h"
#include "myeditor/log.h"

namespace myeditor {

PanelContext::PanelContext(std::shared_ptr<Panel> panel)
  : panel_(std::move(panel)) {
  panel_->SetPanelContext(this);
}

PanelContext::~PanelContext() {
}

bool PanelContext::LoadConf(const Json::Value& conf) {
  config_ = conf;
  if (conf.isMember("show") && conf["show"].isBool()) {
    show_ = conf["show"].asBool();
  }
  if (conf.isMember("display_menu") && conf["display_menu"].isBool()) {
    display_menu_ = conf["display_menu"].asBool();
  }
  return true;
}

const Json::Value* PanelContext::GetConf() const {
  return &config_;
}

bool PanelContext::Init() {
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

void PanelContext::SetModName(const std::string& name) {
  mod_name_ = name;
}

void PanelContext::SetClassName(const std::string& name) {
  class_name_ = name;
}

void PanelContext::SetInstName(const std::string& name) {
  instance_name_ = name;
}

void PanelContext::SetPanelContextManger(PanelContextManager* pcm) {
  panel_ctx_manager_ = pcm;
}

PanelContextManager* PanelContext::GetPanelContextManger() const {
  return panel_ctx_manager_;
}

}  // namespace myeditor
