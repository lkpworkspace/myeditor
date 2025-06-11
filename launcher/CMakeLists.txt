cmake_minimum_required(VERSION 3.10)

set(MYEDITOR_BIN_NAME ${PROJECT_NAME}_launcher)

#### bin
add_executable(${MYEDITOR_BIN_NAME}
  module_argument.cpp
  launcher.cpp
  imgui_impl_glfw.cpp
  imgui_impl_opengl3.cpp
)
target_link_libraries(${MYEDITOR_BIN_NAME}
  PRIVATE
  ${CMAKE_DL_LIBS}
  glfw
  GLEW::glew
  ${PROJECT_NAME}
)
if (glog_lib_type STREQUAL "STATIC_LIBRARY")
  target_link_libraries(${MYEDITOR_BIN_NAME}
    PRIVATE
    glog::glog
  )
endif ()

#### install
install(TARGETS
  ${MYEDITOR_BIN_NAME}
  LIBRARY DESTINATION ${MYEDITOR_LIB_DIR}
  ARCHIVE DESTINATION ${MYEDITOR_LIB_DIR}
  RUNTIME DESTINATION ${MYEDITOR_BIN_DIR}
)
install(DIRECTORY conf DESTINATION .)