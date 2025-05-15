project(nativefiledialog)

# linux
if(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
endif()

add_library(${PROJECT_NAME} STATIC
    src/nfd_common.c
    "$<$<PLATFORM_ID:Linux>:src/nfd_gtk.c>"
    "$<$<PLATFORM_ID:Windows>:src/nfd_win.cpp>"
)
# target_compile_definitions(${PROJECT_NAME}
#     PRIVATE
#     ${GTK3_CFLAGS_OTHER}
# )
target_include_directories(${PROJECT_NAME}
    PRIVATE
    "$<$<PLATFORM_ID:Linux>:${GTK3_INCLUDE_DIRS}>"
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
)
target_link_libraries(${PROJECT_NAME}
    PUBLIC
    "$<$<PLATFORM_ID:Linux>:${GTK3_LIBRARIES}>"
)
