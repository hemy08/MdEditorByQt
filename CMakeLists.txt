cmake_minimum_required(VERSION 3.31)
project(MdEditorByQt)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "D:/Qt/6.10.0/mingw_64/lib/cmake")

set(QT_LIBS Core Gui Widgets Xml)

find_package(Qt6 COMPONENTS ${QT_LIBS} REQUIRED)

include_directories(
        src/common/
        src/log
        src/mainwindow
        src/mainwindow/window_widget
        src/menubar
        src/menubar/base
        src/menubar/hemy_menu
        src/menubar/menu_file
        src/menubar/menu_edit
        src/menubar/menu_view
        src/menubar/menu_coding
        src/menubar/menu_insert
        src/menubar/menu_model
        src/menubar/menu_setting
        src/menubar/menu_tools
        src/menubar/menu_plugins
        src/menubar/menu_help
        src/model
        src/navigation
        src/status_bar
        src/style_def
        src/tools/local
        src/tools/online
        src/tools/web
        src/workbench/compose_bar
        src/workbench/edit_toolbar
        src/workbench/markdown_edit
        src/workbench/markdown_preview
        src/workbench/menu
)

set(SRC_FILES "")
file(GLOB_RECURSE  MENU_SRCS src/mainwindow/window_widget/*.cpp src/menubar/*.cpp src/status_bar/*.cpp src/model/*.cpp src/navigation/*.cpp)
file(GLOB_RECURSE  MAIN_WINDOW_SRCS src/mainwindow/*.cpp src/workbench/*.cpp)
file(GLOB_RECURSE  COMMON_SRCS src/common/*.cpp src/style_def/*.cpp src/log/*.cpp)
file(GLOB_RECURSE  PLG_TOOL_SRCS src/menubar/plugins/*.cpp src/tools/*.cpp)
list(APPEND SRC_FILES ${MENU_SRCS} ${MAIN_WINDOW_SRCS} ${COMMON_SRCS} ${PLG_TOOL_SRCS})
message("SRC_FILES ${SRC_FILES}")

set(HEAD_FILES "")
file(GLOB_RECURSE  MENU_HEAD src/menubar/*.h src/status_bar/*.h)
file(GLOB_RECURSE  MAIN_WINDOW_HEAD src/mainwindow/*.h src/workbench/*.h)
file(GLOB_RECURSE  COMMON_HEAD src/common/*.h src/style_def/*.h)
file(GLOB_RECURSE  PLG_TOOL_HEAD src/menubar/plugins/*.h src/tools/*.h)
list(APPEND HEAD_FILES ${MENU_HEAD} ${MAIN_WINDOW_HEAD} ${COMMON_HEAD} ${PLG_TOOL_HEAD})
message("HEAD_FILES ${HEAD_FILES}")

set(RESOURCE_FILE resources/resource.qrc)

add_executable(MdEditorByQt WIN32 main.cpp ${SRC_FILES} ${HEAD_FILES} ${RESOURCE_FILE})

set(QT_TARGETS)
foreach(lib ${QT_LIBS})
    list(APPEND QT_TARGETS Qt::${lib})
endforeach()
target_link_libraries(MdEditorByQt
        ${QT_TARGETS}
)

# 在代码中定义资源根路径
target_compile_definitions(${PROJECT_NAME} PRIVATE
        RESOURCE_ROOT=":/resources"
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()

    # 拷贝qwindows.dll
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()

    # 拷贝库文件
    foreach (lib IN LISTS QT_LIBS)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${lib}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach ()

    # 拷贝插件文件
    if(EXISTS "${PROJECT_SOURCE_DIR}/hemy_plugins")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${PROJECT_SOURCE_DIR}/hemy_plugins"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/hemy_plugins/")
    endif ()

    # 拷贝自定义工具
    if(EXISTS "${PROJECT_SOURCE_DIR}/cust_tools")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${PROJECT_SOURCE_DIR}/cust_tools"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/cust_tools/")
    endif ()

    # 拷贝资源文件
    if(EXISTS "${PROJECT_SOURCE_DIR}/resources")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${PROJECT_SOURCE_DIR}/resources"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources/")
    endif ()
endif ()

# 添加资源文件
# set(RESOURCE_FILES resources/resources.qrc)
# target_sources(${PROJECT_NAME} PRIVATE ${RESOURCE_FILES})

# 设置翻译文件
# set(TS_FILES
#         translations/app_en_US.xml
#         translations/app_zh_CN.xml
# )

# 自定义函数处理翻译
# include(cmake/FindQtI18n.cmake)
# qt_add_translation(QM_FILES ${TS_FILES})

# 添加自定义目标用于更新翻译
# add_custom_target(update_translations
#         COMMAND Qt6::lupdate ${SOURCE_FILES} menu/*.cpp menu/*.h -ts ${TS_FILES}
#         COMMENT "Updating translation files"
# )

# 安装规则
# install(TARGETS ${PROJECT_NAME} DESTINATION bin)
# install(FILES ${QM_FILES} DESTINATION translations)