#include "imgui.h"
#include "imgui_node_editor.h"
#include "hello_imgui/hello_imgui.h"

namespace ed = ax::NodeEditor;

// A simple utility to get reproducible ids at each frame
struct IdCounter
{
    int _next_id = 0;
    int id()
    {
        _next_id++;
        return _next_id;
    }
    void reset(){
        _next_id = 0;
    }
};


// this is the main Gui function: it draws two nodes and enables to initiate a link
void gui()
{
    static IdCounter next_id;
    static IdCounter next_link_id;
    next_id.reset();
    next_link_id.reset();

    ImGui::SetNextWindowSize(ImVec2(0.0f, 0.0f));
    ed::Begin("My Editor");

    ed::BeginNode(next_id.id());
    ImGui::Text("Node 1");
    ed::BeginPin(next_id.id(), ed::PinKind::Output);
    ImGui::Text("Out ->");
    ed::EndPin();
    ed::EndNode();

    ed::BeginNode(next_id.id());
    ImGui::Text("Node 2");
    ed::BeginPin(next_id.id(), ed::PinKind::Input);
    ImGui::Text("-> In");
    ed::EndPin();
    ed::EndNode();

    if (ed::BeginCreate()) 
    {
        ed::PinId input_pin_id;
        ed::PinId output_pin_id;
        if (ed::QueryNewLink(&input_pin_id, &output_pin_id)) 
        {
            if (ed::AcceptNewItem())
                ed::Link(next_link_id.id(), input_pin_id, output_pin_id);
        }
        ed::EndCreate();
    }

    ed::End();
}


int main(int, char**)
{
    // Prepare the node editor context 
    ed::Config gNodeEditorConfig;
    ed::EditorContext *gNodeEditorContext = ed::CreateEditor(&gNodeEditorConfig);
    ed::SetCurrentEditor(gNodeEditorContext);

    // We use HelloImGui, with a DockSpace (since the issue will occur with dockable windows)
    HelloImGui::RunnerParams runner_params;
    runner_params.imGuiWindowParams.defaultImGuiWindowType = HelloImGui::DefaultImGuiWindowType::ProvideFullScreenDockSpace;
    // runner_params.imGuiWindowParams.enableViewports = true;

    // We create a dockable window in HelloImGui
    HelloImGui::DockableWindow dockableWindow;
    {
        dockableWindow.GuiFunction = gui;
        dockableWindow.label = "graph";
        dockableWindow.dockSpaceName = "MainDockSpace";
    }
    runner_params.dockingParams.dockableWindows = { dockableWindow };

    // Run the app
    HelloImGui::Run(runner_params);

    // Destroy node editor context
    ed::DestroyEditor(gNodeEditorContext);

    return 0;
}
