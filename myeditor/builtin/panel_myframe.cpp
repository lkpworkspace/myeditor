/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#include "myeditor/builtin/panel_myframe.h"
#include <queue>
#include <mutex>
#include "myframe/common.h"
#include "myframe/mod_manager.h"
#include "myeditor/log.h"

namespace {
  std::mutex g_msg2panel_queue_mtx;
  std::queue<std::shared_ptr<myframe::Msg>> g_msg2panel_queue;
}

namespace myeditor {

int PanelProxy::Init(const char* param) {
  return 0;
}

void PanelProxy::Proc(const std::shared_ptr<const myframe::Msg>& msg) {
  // push queue msg
  std::lock_guard<std::mutex> l(g_msg2panel_queue_mtx);
  auto send_msg = std::make_shared<myframe::Msg>();
  send_msg->operator=(*msg);
  g_msg2panel_queue.push(send_msg);
}

// =============================================================================

PanelMyframe::~PanelMyframe() {
  LOG(INFO) << GetPanelName() << " deconstruct";
  if (app_) {
    LOG(INFO) << "myframe quiting...";
    app_->Quit();
  }

  if (th_.joinable()) {
    th_.join();
  }
}

bool PanelMyframe::Init() {
  LOG(INFO) << GetPanelName() << " begin init";
  auto conf = GetConf();

  std::vector<stdfs::path> service_list;
  if (conf->isMember("service")
      && conf->get("service", Json::Value::nullSingleton()).isArray()) {
    // 从配置文件读取加载列表
    const auto& json_service_list = (*conf)["service"];
    for (const auto& service : json_service_list) {
      auto service_conf_file = myframe::Common::GetAbsolutePath(
        service.asString());
      service_list.push_back(service_conf_file);
    }
  } else {
    // 从目录读取加载列表
    auto service_dir = myframe::Common::GetWorkRoot() / "service";
    service_list = myframe::Common::GetDirFiles(service_dir.string());
  }

  // 创建myframe实例
  bool myframe_ok = true;
  app_ = std::make_shared<myframe::App>();
  auto root_dir = myframe::Common::GetWorkRoot();
  auto lib_dir = root_dir / "lib";
  LOG(INFO) << "myframe lib dir " << lib_dir.string();
  if (!app_->Init(lib_dir.string())) {
    LOG(ERROR) << "myframe init failed";
    return false;
  }

  // 注册actor actor.PanelProxy.1
  auto& mod_mgr = app_->GetModManager();
  mod_mgr->RegActor("PanelProxy", [](const std::string&){
    return std::make_shared<PanelProxy>();
  });
  auto a = mod_mgr->CreateActorInst("class", "PanelProxy");
  app_->AddActor("1", "", a);

  // 注册service
  for (size_t i = 0; i < service_list.size(); ++i) {
    if (!app_->LoadServiceFromFile(service_list[i])) {
      LOG(ERROR) << "load service " << service_list[i] << " failed";
      myframe_ok = false;
      app_->Quit();
      break;
    }
  }

  // 异步运行
  th_ = std::thread([this](){
    LOG(INFO) << "myframe runing";
    app_->Exec();
    LOG(INFO) << "myframe quit";
  });

  if (!myframe_ok) {
    if (th_.joinable()) {
      th_.join();
    }
    app_ = nullptr;
    return false;
  }

  LOG(INFO) << GetPanelName() << " init";
  return true;
}

void PanelMyframe::Update() {
  // 发送异步处理完成的消息
  std::lock_guard<std::mutex> l(g_msg2panel_queue_mtx);
  if (g_msg2panel_queue.size() > warning_msg2panel_size_) {
    LOG(WARNING) << "msg to panel size " << g_msg2panel_queue.size();
  }
  while (!g_msg2panel_queue.empty()) {
    auto msg = g_msg2panel_queue.front();
    g_msg2panel_queue.pop();
    Send(msg->GetDesc(), msg);
  }
}

/*
消息格式：
  msg->SetDst("panel.PanelMyframe.1");
  msg->SetType("PROXY");
  msg->SetDesc("actor.xxx.1")
  ...
*/
void PanelMyframe::Proc(const std::shared_ptr<myframe::Msg const>& msg) {
  if (msg->GetType() != "PROXY") {
    return;
  }
  auto dst_panel_name = msg->GetDst();
  auto dst_myframe_name = msg->GetDesc();
  auto send_msg = std::make_shared<myframe::Msg>();
  send_msg->operator=(*msg);
  send_msg->SetDst(dst_myframe_name);
  app_->Send(send_msg);
}

}  // namespace myeditor

// extern "C" std::shared_ptr<myeditor::Panel> panel_create(
//     const std::string& class_name) {
//   if (class_name == "PanelMyframe") {
//     return std::make_shared<myeditor::PanelMyframe>();
//   }
//   return nullptr;
// }
