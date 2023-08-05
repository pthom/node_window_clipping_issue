#include "imgui.h"
#include "imgui_node_editor.h"

namespace ed = ax::NodeEditor;

// External API for this demo
namespace Demo
{
    void Setup();
    void TearDown();
    void Gui();
}

//
// A simple utility to get reproducible ids at each frame
//
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

//
// Utility to store the node editor context
//
namespace NodeContextHolder
{
    struct Holder
    {
        ed::Config gNodeEditorConfig;
        ed::EditorContext *gNodeEditorContext;

        void Setup()
        {
            // Prepare the node editor context
            gNodeEditorContext = ed::CreateEditor(&gNodeEditorConfig);
            ed::SetCurrentEditor(gNodeEditorContext);
        }

        void TearDown()
        {
            // Destroy node editor context
            ed::DestroyEditor(gNodeEditorContext);
        }
    };

    Holder gHolder;
}

//
// Utility to provide a background full screen dockspace
//
namespace DockspaceUtils
{
    // Create a borderless full screen window
    void _CreateFullScreenImGuiWindow()
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);

        ImVec2 viewportSize = viewport->Size;

        ImGui::SetNextWindowSize(viewportSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::SetNextWindowBgAlpha(0.0f);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        static bool p_open = true;
        ImGui::Begin("MainDockSpace", &p_open, window_flags);
        ImGui::PopStyleVar(3);
    }

    // Create a borderless full screen window and embed a dockspace inside
    void BeginFullScreenDockSpace()
    {
        _CreateFullScreenImGuiWindow();
        ImGuiID mainDockspaceId = ImGui::GetID("MainDockSpace");
        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
        ImGui::DockSpace(mainDockspaceId, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    void EndFullScreenDockSpace()
    {
        ImGui::End();
    }

}



namespace Demo
{
    void Setup() 
    { 
        NodeContextHolder::gHolder.Setup(); 
#ifdef _WIN32
        // Incomplete fix for windows HighDPI monitor
        ImGui::GetIO().FontGlobalScale = 2.f;
#endif
    }
    void TearDown() 
    { 
        NodeContextHolder::gHolder.TearDown(); 
    }

    void Gui()
    {

        DockspaceUtils::BeginFullScreenDockSpace();
        ImGui::SetNextWindowDockID(ImGui::GetID("MainDockSpace")); // Comment out this line to trigger bug related to PR 186
                                                                   // (also remove imgui.ini and NodeEditor.json)

        ImGui::Begin("My Demo Window");

        // ImGui::Text("Hello world"); // Uncomment this line to trigger bug related to PR 186

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

        ImGui::End();  // My Demo Window
        DockspaceUtils::EndFullScreenDockSpace();
    }

};
