CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

### lib
file(GLOB builtin_panel_srcs "*.cpp")
add_library(builtin_panel OBJECT ${builtin_panel_srcs})
target_include_directories(builtin_panel
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
  INTERFACE
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(builtin_panel
  PUBLIC
  Threads::Threads
  imgui
  myframe
)

### install
file(GLOB conf_files "*.json")
install(FILES
  ${conf_files}
  PERMISSIONS
  OWNER_READ OWNER_WRITE
  GROUP_READ
  WORLD_READ
  DESTINATION ${MYEDITOR_PANEL_DIR}
)
