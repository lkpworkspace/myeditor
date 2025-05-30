cmake_minimum_required(VERSION 3.10)
if (POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW)
endif()
if (POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif()
project(myeditor VERSION 1.0.0)

### option

### cmake module
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

### compile option
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if (CMAKE_CXX_STANDARD_REQUIRED)
  message(STATUS "Set cxx standard ${CMAKE_CXX_STANDARD}")
else()
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_STANDARD 17)
  message(STATUS "Set default cxx standard 17")
endif()
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")

### install dir
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  message(STATUS "Set default install prefix ${CMAKE_INSTALL_PREFIX}")
else()
  message(STATUS "Set install prefix ${CMAKE_INSTALL_PREFIX}")
endif()
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(MYEDITOR_LIB_DIR "lib")
set(MYEDITOR_BIN_DIR "bin")
set(MYEDITOR_INC_DIR "include")
set(MYEDITOR_CONF_DIR "conf")
set(MYEDITOR_PANEL_DIR "panel")

### deps libs
find_package(OpenGL REQUIRED)
find_package(glew REQUIRED)
find_package(glfw3 REQUIRED)
find_package(myframe REQUIRED)
find_package(imgui REQUIRED)

### editor
add_subdirectory(3rd/nativefiledialog)
add_subdirectory(3rd/stb)
add_subdirectory(myeditor)
add_subdirectory(launcher)
