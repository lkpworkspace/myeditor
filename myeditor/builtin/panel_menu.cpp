/****************************************************************************
Copyright (c) 2025, 李柯鹏
All rights reserved.

Author: 李柯鹏 <likepeng0418@163.com>
****************************************************************************/
#include "myeditor/builtin/panel_menu.h"

namespace myeditor {

PanelMenu::~PanelMenu() {}

bool PanelMenu::Init() { return true; }

void PanelMenu::Update() {
  static bool create_new = false;
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New", "")) {
        create_new = true;
      }
      if (ImGui::MenuItem("Open...", "")) {
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Save as...", "CTRL+SHIFT+S")) {
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}

}  // namespace myeditor
