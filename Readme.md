# Example code for reproducing https://github.com/thedmd/imgui-node-editor/issues/205

## Build instructions

```
./fetch_external.sh
mkdir build
cd build
cmake ..
make -j
```

## Code explanation

It was not possible to use the ImGui version inside node editor, since it is outdated, hence this separate repository.

The code is contained in two short files: 
* [main_imgui_sdl_opengl3.cpp](main_imgui_sdl_opengl3.cpp): This file is an almost direct copy of  imgui/examples/example_glfw_opengl3/main.cpp, the only difference being that it calls functions from node_clipping_issue.cpp (for the Gui and context setup)

* [node_clipping_issue.cpp](node_clipping_issue.cpp)
Demo code. 

## Issues explanations

This demo is able to trigger subtle issues that arise when using docked windows,
and/or windows whose size is unknown at the first frame (so that the canvas does not know its size)

4 issues can be triggered: they are detailed below. I'm sorry if all of them are grouped in a single project, but it seemed to be the most efficient way to communicate about them. Most of the fixes are simple, but the fix for issue 205 is only a workaround.

#### Issue 205: https://github.com/thedmd/imgui-node-editor/issues/205
> Clipping is not related to the window with the latest imgui"

Adding a simple pixel at m_WidgetRect.Max is enough to bypass the issue.
This is not a real fix, since it merely adds a fake pixel at (xmax, ymax), so that the app knows where the clipping should end. A better solution would be required.

*Workaround: at imgui_canvas.cpp:122 (see [commit](https://github.com/pthom/imgui-node-editor/commit/2265a4bad141a07478292186eb282c85a4bb6991))*

```cpp
bool ImGuiEx::Canvas::Begin(ImGuiID id, const ImVec2& size)
{
    ...
    ...
   // Adding a simple pixel at m_WidgetRect.Max is enough to temporarily solve the issue
   // (the color is selected to be as transparent as possible)
   m_DrawList->AddLine(m_WidgetRect.Max, m_WidgetRect.Max, IM_COL32(0, 0, 0, 1)); // <= **** Added this workaround ****
```

#### PR "186" : https://github.com/thedmd/imgui-node-editor/pull/186
>if ImGuiEx::Canvas::Begin returns false (which can happen if the window is too small and the canvas is Clipped), then the subsequent call to ImGuiEx::Canvas::End() will trigger an IM_ASSERT

This bug is quite complex to trigger:

- the window size must be still unknown at first display
- you may need to remove imgui.ini and NodeEditor.json to trigger it so that the window size is actually unknown
- a widget must be displayed before the node editor (see ImGui::Text("Hello world") below)

* Fix: at imgui_node_editor.cpp:1126 (see [commit](https://github.com/pthom/imgui-node-editor/commit/2ba8ca4f3a36ee086950d98b34422c81e550722c))*
```cpp
void ed::EditorContext::Begin(const char* id, const ImVec2& size)
{
...

         if (!m_IsInitialized)
         {
            // Cycle canvas so it has a change to setup its size before settings are loaded
            if (m_Canvas.Begin(id, canvasSize))     // <== **** Added this if ****
                m_Canvas.End();
            LoadSettings();
            m_IsInitialized = true;
         }
```

#### Issue with bool ed::CreateItemAction::Begin()  (not mentioned in the repo issues yet)
> ed::CreateItemAction::Begin: if (m_CurrentStage == None) => m_InActive = false
Otherwise, we may reach IM_ASSERT(false == m_InActive) upon next call. This happens at first display

*Fix: at imgui_node_editor.cpp:4710 (see [commit](https://github.com/pthom/imgui-node-editor/commit/2ba8ca4f3a36ee086950d98b34422c81e550722c))*

```cpp
bool ed::CreateItemAction::Begin()
{
    ...
    ...
    if (m_CurrentStage == None)
    {
        m_InActive = false;       // <== **** Added this statement ****
        return false;
    }
```


#### [remove duplicate ImVec2 operator-](https://github.com/pthom/imgui-node-editor/commit/0da24eb1bbc4355113509ab519d37b1f856660c0)
(operator- is now provided by imgui)
