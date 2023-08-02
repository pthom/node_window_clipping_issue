cmake_minimum_required(VERSION 3.12)
project(node_clipping_issue)
set(CMAKE_CXX_STANDARD 17)

if (NOT IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/external/imgui)
    message(FATAL_ERROR "Please run ./fetch_external.sh in order to populate external/")
endif()

##########################################################
# Add ImGui & HelloImGui
##########################################################
# i. HelloImGui will build ImGui as part of its build (and here, we tell it to use our own version in external/imgui)
set(HELLOIMGUI_IMGUI_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/external/imgui")
add_subdirectory(external/hello_imgui)
# ii. HelloImGui provides a function `hello_imgui_add_app` available in order to easily add applications
list(APPEND CMAKE_MODULE_PATH ${HELLOIMGUI_CMAKE_PATH})
include(hello_imgui_add_app)


##########################################################
# Add imgui-node-editor
##########################################################
set(imgui_node_editor_dir ${CMAKE_CURRENT_LIST_DIR}/external/imgui-node-editor)
file(GLOB imgui_node_editor_src ${imgui_node_editor_dir}/*.cpp ${imgui_node_editor_dir}/*.h)
add_library(imgui_node_editor ${imgui_node_editor_src})
target_include_directories(imgui_node_editor PUBLIC ${imgui_node_editor_dir})
target_link_libraries(imgui_node_editor PUBLIC imgui)

##########################################################
# Build your app
##########################################################
hello_imgui_add_app(node_clipping_issue node_clipping_issue.cpp)
target_link_libraries(node_clipping_issue PRIVATE imgui_node_editor)
