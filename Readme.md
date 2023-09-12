# Example code for reproducing an issue with suspend and resume and SentinelDrawCallback() in imgui-node-editor

Demo video: https://traineq.org/poub/node_editor_issue_suspend.mp4

## Build instructions

```
./fetch_external.sh
mkdir build
cd build
cmake ..
make -j
```


## Code explanation

The code is contained in two short files: 
* [main_imgui_sdl_opengl3.cpp](main_imgui_sdl_opengl3.cpp): This file is an almost direct copy of  imgui/examples/example_glfw_opengl3/main.cpp, the only difference being that it calls functions from node_clipping_issue.cpp (for the Gui and context setup)

* [suspend_resume_issue.cpp](suspend_resume_issue.cpp)
Demo code. 

## Issue explanations

This code will call ax::NodeEditor::Detail::EditorContext::Suspend() & Resume()

The issue is that SentinelDrawCallback may be called and will trigger an assert.

```cpp
static void SentinelDrawCallback(const ImDrawList* parent_list, const ImDrawCmd* cmd)
{
    // This is a sentinel draw callback, it's only purpose is to mark draw list command.
    IM_ASSERT(false && "This draw callback should never be called.");
}
```

It can be triggered by minimizing the window "Main window".
