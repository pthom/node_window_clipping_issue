cmake_minimum_required(VERSION 3.12)
project(node_clipping_issue)
set(CMAKE_CXX_STANDARD 17)

if (NOT IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/external/imgui)
    message(FATAL_ERROR "Please run ./fetch_external.sh in order to populate external/")
endif()


##########################################################
# Add SDL3
##########################################################
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(external/SDL)


##########################################################
# Add ImGui (with SDL3 + OpenGl3 backend)
##########################################################
find_package(OpenGL REQUIRED)
set(imgui_dir ${CMAKE_CURRENT_LIST_DIR}/external/imgui)
file(GLOB imgui_sources
    ${imgui_dir}/*.h ${imgui_dir}/*.cpp
    ${imgui_dir}/backends/imgui_impl_sdl3*
    ${imgui_dir}/backends/imgui_impl_opengl3*
    )
add_library(imgui_sdl3_opengl3 ${imgui_sources})
target_include_directories(imgui_sdl3_opengl3 PUBLIC ${imgui_dir})
target_include_directories(imgui_sdl3_opengl3 PUBLIC ${imgui_dir}/backends)
target_link_libraries(imgui_sdl3_opengl3 PUBLIC SDL3-static ${OPENGL_LIBRARIES})

##########################################################
# Add imgui-node-editor
##########################################################
set(imgui_node_editor_dir ${CMAKE_CURRENT_LIST_DIR}/external/imgui-node-editor)
file(GLOB imgui_node_editor_src ${imgui_node_editor_dir}/*.cpp ${imgui_node_editor_dir}/*.h)
add_library(imgui_node_editor ${imgui_node_editor_src})
target_include_directories(imgui_node_editor PUBLIC ${imgui_node_editor_dir})
target_link_libraries(imgui_node_editor PUBLIC imgui_sdl3_opengl3)

##########################################################
# Build app
##########################################################
add_executable(suspend_resume_issue.cpp main_imgui_sdl3_opengl3.cpp suspend_resume_issue.cpp)
target_link_libraries(suspend_resume_issue.cpp PRIVATE imgui_sdl3_opengl3 imgui_node_editor)
