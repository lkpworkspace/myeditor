/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include <memory>
#include <string>

#include <json/json.h>

#include "myframe/msg.h"
#include "myeditor/export.h"

namespace myeditor {

class PanelContext;
class MYEDITOR_EXPORT Panel {
  friend class PanelContext;

 public:
  Panel() = default;
  virtual ~Panel() {}

  /**
   * Init() - 对象创建初始化函数
   * @return: 成功返回true，失败返回false
   */
  virtual bool Init() { return true; }

  /**
   * Send() - 发送消息
   * @return: 成功返回true，失败返回false
   */
  void Send(const std::string& dst, std::shared_ptr<myframe::Msg> msg);

 protected:
  /**
   * Update() - 每帧更新处理函数
   *
   */
  virtual void Update() {}

  /**
   * Proc() - 消息处理函数
   * @msg:      收到的消息
   *
   */
  virtual void Proc(const std::shared_ptr<myframe::Msg const>&) {}

  /**
   * Render() - 帧渲染函数
   *
   */
  virtual void Render() {}

 private:
  PanelContext* ctx_{nullptr};
};

}  // namespace myeditor

#include "myframe/platform.h"
#if defined(MYFRAME_OS_WINDOWS)
template class std::shared_ptr<myeditor::Panel>;
#endif
extern "C" {
typedef std::shared_ptr<myeditor::Panel> (*panel_create_func_t)(
    const std::string&);
}  // extern "C"
