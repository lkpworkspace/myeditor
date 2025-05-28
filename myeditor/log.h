/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include <string>
#include <functional>

#include <glog/logging.h>

#include "myframe/common.h"
#include "myeditor/export.h"

namespace myeditor {

MYEDITOR_EXPORT void InitLog(
  const stdfs::path& log_dir,
  const std::string& bin_name,
  int max_size_mb = 100);

MYEDITOR_EXPORT void RegisterWriteCallback(
  std::function<void(const std::string&)> func);

MYEDITOR_EXPORT void UnRegisterWriteCallback();

MYEDITOR_EXPORT void ShutdownLog();

}  // namespace myeditor
