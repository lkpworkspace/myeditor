/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#include "myeditor/mod_manager.h"

#include "myframe/shared_library.h"

#include "myeditor/log.h"
#include "myeditor/panel.h"
#include "myeditor/panel_context.h"

namespace myeditor {

ModManager::ModManager(const std::string& lib_dir) {
  LOG(INFO) << "ModManager create";
  lib_dir_ = myframe::Common::GetAbsolutePath(lib_dir);
}

ModManager::~ModManager() {
  LOG(INFO) << "ModManager deconstruct";
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

std::shared_ptr<Panel> ModManager::CreatePanel(
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

bool ModManager::CreatePanelContext(const Json::Value& config,
  std::vector<std::shared_ptr<PanelContext>>* panel_ctx_list) {
  if (panel_ctx_list == nullptr) {
    LOG(ERROR) << "panel_ctx_list is nullptr";
    return false;
  }
  if (!config.isMember("type")) {
    LOG(ERROR) << "no type member";
    return false;
  }
  if (!config["type"].isString()) {
    LOG(ERROR) << "type member not string";
    return false;
  }
  std::string type = config["type"].asString();
  std::string lib_name;
  // load lib
  if (type != "class") {
    if (!config.isMember("lib")) {
      LOG(ERROR) << "no lib member";
      return false;
    }
    if (!config["lib"].isString()) {
      LOG(ERROR) << "lib member not string";
      return false;
    }
    lib_name = config["lib"].asString();
    lib_name = myframe::Common::GetLibName(lib_name);
    if (!LoadMod(lib_dir_ / lib_name)) {
      LOG(ERROR) << "load " << lib_name << "failed";
      return false;
    }
  }

  // create panel instance
  if (!config.isMember("panel")) {
    LOG(INFO) << "no panel conf, skip";
    return true;
  }
  if (!config["panel"].isObject()) {
    LOG(ERROR) << "panel member not obj";
    return false;
  }
  const auto& panel_list = config["panel"];
  Json::Value::Members panel_class_name_list = panel_list.getMemberNames();
  for (auto class_name_it = panel_class_name_list.begin();
        class_name_it != panel_class_name_list.end(); ++class_name_it) {
    std::string panel_class_name = *class_name_it;
    auto panel_inst_list = panel_list[panel_class_name];
    for (const auto& inst : panel_inst_list) {
      if (!inst.isMember("name")) {
        LOG(ERROR)
          << "panel " << panel_class_name
          << " key \"name\": no key, skip";
        return false;
      }
      std::shared_ptr<Panel> panel = nullptr;
      if (type == "class") {
        panel = CreatePanel(type, panel_class_name);
      } else if (type == "library") {
        panel = CreatePanel(lib_name, panel_class_name);
      } else {
        LOG(ERROR) << "Unknown type" << type;
        return false;
      }
      if (panel == nullptr) {
        LOG(ERROR) << "Create panel failed";
        return false;
      }
      LOG(INFO) << "load instance panel."
        << panel_class_name << " "
        << inst.toStyledString(); 
      // create panel context instance
      auto panel_ctx = std::make_shared<PanelContext>(std::move(panel));
      panel_ctx->SetModName(type == "class" ? "class" : lib_name);
      panel_ctx->SetClassName(panel_class_name);
      panel_ctx->SetInstName(inst["name"].asString());
      panel_ctx->LoadConf(inst["config"]);
      panel_ctx_list->push_back(panel_ctx);
    }
  }
  return true;
}

}  // namespace myeditor
