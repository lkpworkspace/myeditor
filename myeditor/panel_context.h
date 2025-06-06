/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include <list>
#include <memory>

#include <json/json.h>

#include "myframe/msg.h"

namespace myeditor {

class Panel;
class PanelContextManager;
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
  void SetModName(const std::string&);
  void SetClassName(const std::string&);
  void SetInstName(const std::string&);

  bool IsShow() const;
  void SetShow(bool);
  bool IsDisplayMenu() const;
  void SetDisplayMenu(bool b = false);

  bool LoadConf(const Json::Value&);
  const Json::Value* GetConf() const;

  PanelContextManager* GetPanelContextManger() const;
  void SetPanelContextManger(PanelContextManager*);

 private:
  bool show_{true};
  bool display_menu_{true};
  Json::Value config_;

  std::string mod_name_;
  std::string class_name_;
  std::string instance_name_;

  PanelContextManager* panel_ctx_manager_{nullptr};

  std::list<std::shared_ptr<myframe::Msg>> recv_;
  std::shared_ptr<Panel> panel_;
};

}  // namespace myeditor
