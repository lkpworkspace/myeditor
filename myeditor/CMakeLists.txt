cmake_minimum_required(VERSION 3.10)

add_subdirectory(builtin)

#### library
aux_source_directory(. myeditor_srcs)
add_library(${PROJECT_NAME} SHARED
  ${myeditor_srcs}
  $<TARGET_OBJECTS:builtin_panel>
)
target_include_directories(${PROJECT_NAME}
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  INTERFACE
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
  nativefiledialog
  PUBLIC
  Threads::Threads
  GLEW::glew
  imgui
  myframe
)

# export file
include (GenerateExportHeader)
generate_export_header (${PROJECT_NAME}
  EXPORT_MACRO_NAME MYEDITOR_EXPORT
  EXPORT_FILE_NAME "${CMAKE_CURRENT_SOURCE_DIR}/export.h"
)

#### install
set(myeditor_headers
  export.h
  rate_ctrl.h
  panel.h
  mod_manager.h
  editor.h
)
install(FILES
  ${myeditor_headers}
  PERMISSIONS
  OWNER_READ
  GROUP_READ
  WORLD_READ
  DESTINATION ${MYEDITOR_INC_DIR}/${PROJECT_NAME}
)
install(TARGETS ${PROJECT_NAME}
  EXPORT "${PROJECT_NAME}Targets"
  LIBRARY DESTINATION ${MYEDITOR_LIB_DIR}
  ARCHIVE DESTINATION ${MYEDITOR_LIB_DIR}
  RUNTIME DESTINATION ${MYEDITOR_BIN_DIR}
)
