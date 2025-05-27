/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#include "myeditor/panel.h"

#include "myeditor/panel_context.h"
#include "myeditor/panel_context_manager.h"

namespace myeditor {

void Panel::Send(const std::string& dst, std::shared_ptr<myframe::Msg> msg) {
  // TODO Send message to other panel
}

} // namespace myeditor