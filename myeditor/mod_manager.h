/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include <mutex>
#include <shared_mutex>
#include <functional>
#include <memory>
#include <unordered_map>
#include <string>

#include <json/json.h>

#include "myframe/macros.h"
#include "myframe/common.h"
#include "myframe/shared_library.h"

#include "myeditor/export.h"
#include "myeditor/panel.h"

namespace myeditor {

class Panel;
class MYEDITOR_EXPORT ModManager final {
 public:
  ModManager(const std::string& lib_dir = "lib");
  virtual ~ModManager();

  bool RegPanel(
    const std::string& class_name,
    std::function<std::shared_ptr<Panel>(const std::string&)> func);

  bool CreatePanelContext(
    const Json::Value& config,
    std::vector<std::shared_ptr<PanelContext>>* panel_ctx_list);

 private:
  bool LoadMod(const std::string& dl_path);

  std::shared_ptr<Panel> CreatePanel(
    const std::string& mod_or_class_name,
    const std::string& panel_name);

  stdfs::path lib_dir_;

  std::shared_mutex class_panel_rw_;
  std::unordered_map<
      std::string, std::function<std::shared_ptr<Panel>(const std::string&)>>
      class_panels_;

  std::shared_mutex mods_rw_;
  std::unordered_map<std::string, std::shared_ptr<myframe::SharedLibrary>> mods_;

  DISALLOW_COPY_AND_ASSIGN(ModManager)
};

}  // namespace myframe
