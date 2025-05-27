/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#include "myeditor/mod_manager.h"

#include "myframe/common.h"
#include "myframe/shared_library.h"

#include "myeditor/log.h"
#include "myeditor/panel.h"

namespace myeditor {

ModManager::ModManager() {
  LOG(INFO) << "ModManager create";
}

ModManager::~ModManager() {
  LOG(INFO) << "ModManager deconstruct";
}

bool ModManager::LoadPanelContext(const Json::Value& config,
  std::vector<std::shared_ptr<PanelContext>>* panel_ctx_list) {
  // TODO load panel inst from config
  return true;
}

bool ModManager::LoadMod(const std::string& dl_path) {
  auto dlname = stdfs::path(dl_path).filename().string();
  std::unique_lock<std::shared_mutex> lk(mods_rw_);
  if (mods_.find(dlname) != mods_.end()) {
    VLOG(1) << dlname << " has loaded";
    return true;
  }
  auto lib = myframe::SharedLibrary::Create();
  if (!lib->Load(dl_path, myframe::SharedLibrary::Flags::kLocal)) {
    return false;
  }
  mods_[dlname] = lib;
  LOG(INFO) << "Load lib " << dl_path;
  return true;
}

bool ModManager::RegPanel(
  const std::string& class_name,
  std::function<std::shared_ptr<Panel>(const std::string&)> func) {
  std::unique_lock<std::shared_mutex> lk(class_panel_rw_);
  if (class_panels_.find(class_name) != class_panels_.end()) {
    LOG(WARNING)
      << "reg " << class_name << " failed, "
      << " has exist";
    return false;
  }
  class_panels_[class_name] = func;
  return true;
}

std::shared_ptr<Panel> ModManager::CreatePanelInst(
  const std::string& mod_or_class_name, const std::string& panel_name) {
  {
    std::shared_lock<std::shared_mutex> lk(mods_rw_);
    if (mods_.find(mod_or_class_name) != mods_.end()) {
      VLOG(1) << panel_name << " actor from lib";
      auto lib = mods_[mod_or_class_name];
      auto void_func = lib->GetSymbol("panel_create");
      auto create = reinterpret_cast<panel_create_func_t>(void_func);
      if (nullptr == create) {
        LOG(ERROR)
          << "Load " << mod_or_class_name << "." << panel_name
          << " module actor_create function failed";
        return nullptr;
      }
      auto panel = create(panel_name);
      if (nullptr == panel) {
        LOG(ERROR)
          << "Create " << mod_or_class_name << "." << panel_name
          << " failed";
        return nullptr;
      }
      return panel;
    }
  }
  std::shared_lock<std::shared_mutex> lk(class_panel_rw_);
  if (mod_or_class_name == "class" &&
      class_panels_.find(panel_name) != class_panels_.end()) {
    VLOG(1) << panel_name << " panel from reg class";
    auto panel = class_panels_[panel_name](panel_name);
    return panel;
  }
  return nullptr;
}

}  // namespace myeditor
