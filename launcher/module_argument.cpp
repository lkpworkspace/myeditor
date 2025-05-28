/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#include "module_argument.h"
#include "myframe/platform.h"
#if defined(MYFRAME_OS_LINUX) || defined(MYFRAME_OS_ANDROID) \
    || defined(MYFRAME_OS_MACOSX)
#include <unistd.h>
#elif defined(MYFRAME_OS_WINDOWS)
#include <process.h>
#endif
#include <iostream>

namespace myeditor {

ModuleArgument::ModuleArgument(
    const std::string& default_sys_conf_dir) {
  default_sys_conf_dir_ = myframe::Common::GetAbsolutePath(
    default_sys_conf_dir);
  parser_.add<std::string>("process-name", 'p',
    "The name of this launcher process, "
    "and it is also the name of log, "
    "default value is myeditor_launcher_${PID}\n",
    false, "");
  parser_.add<std::string>("sys-conf", 's',
    "editor config file",
    false, "myeditor.json");
  parser_.add<std::string>("dir", 'd',
    "editor panel config dir",
    false, "");
  parser_.add<std::string>("log-dir", 0,
    "editor log dir",
    false, "");
  parser_.add<std::string>("lib-dir", 0,
    "editor lib dir",
    false, "");
  parser_.footer("panel_config_file ...");
}

void ModuleArgument::ParseArgument(
  const int argc, char** argv) {
  binary_name_ = stdfs::path(argv[0]).filename().string();
  process_name_ = binary_name_ + "_" + std::to_string(getpid());

  // log command for info
  std::string cmd("");
  for (int i = 0; i < argc; ++i) {
    cmd += argv[i];
    cmd += " ";
  }
  cmd_ = cmd;

  // check args
  parser_.parse_check(argc, argv);

  auto process_name = parser_.get<std::string>("process-name");
  if (!process_name.empty()) {
    process_name_ = process_name;
  }

  // 命令行参数优先级大于配置文件优先级
  auto sys_conf = parser_.get<std::string>("sys-conf");
  if (!sys_conf.empty()) {
    if (!ParseSysConf(sys_conf)) {
      std::cerr << "parse sys conf failed!!" << std::endl;
      exit(-1);
    }
  }

  auto dir = parser_.get<std::string>("dir");
  if (!dir.empty()) {
    panel_conf_dir_ = dir;
  }

  auto log_dir = parser_.get<std::string>("log-dir");
  if (!log_dir.empty()) {
    log_dir_ = log_dir;
  }

  auto lib_dir = parser_.get<std::string>("lib-dir");
  if (!lib_dir.empty()) {
    lib_dir_ = lib_dir;
  }

  for (size_t i = 0; i < parser_.rest().size(); i++) {
    panel_conf_list_.emplace_back(parser_.rest()[i]);
  }
}

bool ModuleArgument::ParseSysConf(const std::string& sys_conf) {
  std::string full_sys_conf;
  if (myframe::Common::IsAbsolutePath(sys_conf)) {
    full_sys_conf = sys_conf;
  } else {
    full_sys_conf = (default_sys_conf_dir_ / sys_conf).string();
  }
  auto root = myframe::Common::LoadJsonFromFile(full_sys_conf);
  if (root.isNull()
      || !root.isObject()) {
    return false;
  }
  if (root.isMember("name")
      && root["name"].isString()) {
    process_name_ = root["name"].asString();
  }
  if (root.isMember("log_dir")
      && root["log_dir"].isString()) {
    log_dir_ = root["log_dir"].asString();
  }
  if (root.isMember("lib_dir")
      && root["lib_dir"].isString()) {
    lib_dir_ = root["lib_dir"].asString();
  }
  if (root.isMember("panel_conf_dir")
      && root["panel_conf_dir"].isString()) {
    panel_conf_dir_ = root["panel_conf_dir"].asString();
  }
  if (root.isMember("log_max_size_mb")
      && root["log_max_size_mb"].isInt()) {
    log_max_size_mb_ = root["log_max_size_mb"].asInt();
  }
  return true;
}

std::ostream& operator<<(std::ostream& out, const ModuleArgument& ma) {
  out << "myeditor config\n"
    << "|-- cmd: " << ma.cmd_ << "\n"
    << "|-- binary name: " << ma.binary_name_ << "\n"
    << "|-- process name: " << ma.process_name_ << "\n"
    << "|-- sys conf dir: " << ma.default_sys_conf_dir_ << "\n"
    << "|-- lib dir: " << ma.lib_dir_ << "\n"
    << "|-- log dir: " << ma.log_dir_ << "\n"
    << "|-- panel conf dir: " << ma.panel_conf_dir_ << "\n";
    // panel conf files
    if (!ma.panel_conf_list_.empty()) {
      out << "|-- panel conf files:\n";
    }
    for (const auto& panel_conf_file : ma.panel_conf_list_) {
      out << "    |-- " << panel_conf_file << "\n";
    }
  return out;
}

}  // namespace myeditor
