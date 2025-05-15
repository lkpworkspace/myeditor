/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include <list>
#include <string>

#include "cmdline.h"
#include "myframe/common.h"

namespace myeditor {

class ModuleArgument final {
  friend std::ostream& operator<<(std::ostream&, const ModuleArgument&);
 public:
  ModuleArgument(const std::string& default_sys_conf_dir = "conf");
  ~ModuleArgument() = default;

  void ParseArgument(const int argc, char** argv);
  inline std::list<std::string> GetPanelConfList() const {
    return panel_conf_list_;
  }
  inline std::string GetPanelConfDir() const { return panel_conf_dir_; }
  inline std::string GetLogDir() const { return log_dir_; }
  inline std::string GetLibDir() const { return lib_dir_; }
  inline std::string GetBinaryName() const { return binary_name_; }
  inline std::string GetProcessName() const { return process_name_; }
  inline std::string GetCmd() const { return cmd_; }
  inline int GetLogMaxSizeMB() const { return log_max_size_mb_; }

 private:
  bool ParseSysConf(const std::string&);

  int log_max_size_mb_{100};
  std::string log_dir_;
  std::string lib_dir_;
  std::string panel_conf_dir_;
  std::list<std::string> panel_conf_list_;
  stdfs::path default_sys_conf_dir_;

  std::string cmd_;
  std::string binary_name_;
  std::string process_name_;

  cmdline::parser parser_;
};

std::ostream& operator<<(std::ostream&, const ModuleArgument&);

}  // namespace myeditor
