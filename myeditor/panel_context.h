/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include <json/json.h>

#include <list>
#include <memory>

#include "myframe/msg.h"

namespace myeditor {

class Panel;
class PanelContext {
 public:
  PanelContext(std::shared_ptr<Panel> panel);
  virtual ~PanelContext();

  bool Init();

  void Update();

  void ProcMessage();

  void Render();

  void RecvMessage(std::shared_ptr<myframe::Msg> msg);

  std::string GetPanelName() const;

  bool IsShow() const;
  void SetShow(bool);
  bool IsDisplayMenu() const;
  void SetDisplayMenu(bool b = false);

 private:
  bool show_{true};
  bool display_menu_{true};
  std::string mod_name_;
  std::string class_name_;
  std::string instance_name_;
  Json::Value config_;

  std::list<std::shared_ptr<myframe::Msg>> recv_;
  std::shared_ptr<Panel> panel_;
};

}  // namespace myeditor
