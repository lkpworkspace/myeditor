/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include <thread>
#include <mutex>

#include "myframe/app.h"
#include "myframe/actor.h"

#include "myeditor/panel.h"

namespace myeditor {

/*
  myframe发送给panel消息的具体格式
    msg->SetDst("actor.PanelProxy.1");
    msg->SetType("PROXY");  // 标记目的地址是代理地址
    msg->SetDesc("panel.xxx.1");  // 这个是实际目的地址
    ...

  panel发送给myframe消息的具体格式：
    msg->SetDst("panel.PanelMyframe.1");
    msg->SetType("PROXY");
    msg->SetDesc("actor.xxx.1")
    ...
*/
class PanelProxy : public myframe::Actor {
 public:
  int Init(const char* param) override;
  void Proc(const std::shared_ptr<const myframe::Msg>& msg);
};

class PanelMyframe : public myeditor::Panel {
 public:
  virtual ~PanelMyframe();

  bool Init() override;

  void Update() override;

  void Proc(const std::shared_ptr<myframe::Msg const>&);

 private:
  int warning_msg2panel_size_{10};
  std::shared_ptr<myframe::App> app_;
  std::thread th_;
};

}  // namespace myeditor
