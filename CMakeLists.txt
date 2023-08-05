cmake_minimum_required(VERSION 3.12)
project(node_clipping_issue)
set(CMAKE_CXX_STANDARD 17)

if (NOT IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/external/imgui)
    message(FATAL_ERROR "Please run ./fetch_external.sh in order to populate external/")
endif()

##########################################################
# Add glfw
##########################################################
if(APPLE)
    enable_language(OBJC) # workaround glfw build issue on macOS
endif()
add_subdirectory(external/glfw)

##########################################################
# Add ImGui (with glfw + OpenGl3 backend)
##########################################################
find_package(OpenGL REQUIRED)
set(imgui_dir ${CMAKE_CURRENT_LIST_DIR}/external/imgui)
file(GLOB imgui_sources
    ${imgui_dir}/*.h ${imgui_dir}/*.cpp
    ${imgui_dir}/backends/imgui_impl_glfw*
    ${imgui_dir}/backends/imgui_impl_opengl3*
    )
add_library(imgui_glfw_opengl3 ${imgui_sources})
target_include_directories(imgui_glfw_opengl3 PUBLIC ${imgui_dir})
target_include_directories(imgui_glfw_opengl3 PUBLIC ${imgui_dir}/backends)
target_link_libraries(imgui_glfw_opengl3 PUBLIC glfw ${OPENGL_LIBRARIES})

##########################################################
# Add imgui-node-editor
##########################################################
set(imgui_node_editor_dir ${CMAKE_CURRENT_LIST_DIR}/external/imgui-node-editor)
file(GLOB imgui_node_editor_src ${imgui_node_editor_dir}/*.cpp ${imgui_node_editor_dir}/*.h)
add_library(imgui_node_editor ${imgui_node_editor_src})
target_include_directories(imgui_node_editor PUBLIC ${imgui_node_editor_dir})
target_link_libraries(imgui_node_editor PUBLIC imgui_glfw_opengl3)

##########################################################
# Build app
##########################################################
add_executable(node_clipping_issue main_imgui_glfw_opengl3.cpp node_clipping_issue.cpp)
target_link_libraries(node_clipping_issue PRIVATE imgui_glfw_opengl3 imgui_node_editor)
