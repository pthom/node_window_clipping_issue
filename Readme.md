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

This code is self-contained in a short file: [node_clipping_issue.cpp](node_clipping_issue.cpp)

It uses HelloImGui in order to simplify the example creation. HelloImGui is based on a quite recent version of the ImGui docking branch.

It was not possible to use the ImGui version inside node editor, since it is outdated, and adds stack layout patches to ImGui which are not trivial to rebase.

## Fixes applied to imgui-node-editor (based on the develop branch)

#### [remove duplicate ImVec2 operator-](https://github.com/pthom/imgui-node-editor/commit/0da24eb1bbc4355113509ab519d37b1f856660c0)
(operator- is now provided by imgui)

#### [ed::CreateItemAction::Begin: if (m_CurrentStage == None) => m_InActive = false](https://github.com/pthom/imgui-node-editor/commit/2ba8ca4f3a36ee086950d98b34422c81e550722c)
(Undo this commit to see the related issue: execution will fail at startup)

Workaround another unrelated issue that is triggered by this example.

#### [Fix #205: workaround clipping issues when link with docked windows](https://github.com/pthom/imgui-node-editor/commit/2265a4bad141a07478292186eb282c85a4bb6991)
(Undo this commit to see the issue: clipping issues will appear when creating links)

This is a workaround for
https://github.com/thedmd/imgui-node-editor/issues/205
https://github.com/pthom/imgui_bundle/issues/117
Adding a simple pixel at m_WidgetRect.Max is enough to bypass the issue.

This is not a real fix, since it merely adds a fake pixel at (xmax, ymax), so that the app knows where the clipping should end. A better solution would be required.
