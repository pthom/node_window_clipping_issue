#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_node_editor.h"
#include "imgui_node_editor_internal.h"

namespace ed = ax::NodeEditor;

// External API for this demo
namespace Demo
{
    void Setup();
    void TearDown();
    void Gui();
}

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

        void Suspend()
        {
            auto context_cast = (ax::NodeEditor::Detail::EditorContext *)gNodeEditorContext;
            context_cast->Suspend();
        }
        void Resume()
        {
            auto context_cast = (ax::NodeEditor::Detail::EditorContext *)gNodeEditorContext;
            context_cast->Resume();
        }
    };

    Holder gHolder;
}



namespace Demo
{
    void Setup() 
    { 
        NodeContextHolder::gHolder.Setup(); 
    }
    void TearDown() 
    { 
        NodeContextHolder::gHolder.TearDown(); 
    }

    void BeginMainWindow()
    {
#if 1   // Either we open a standard window, and the bug appears when we minimize it
        ImGui::Begin("Main window");
#else   // or we open a fullscreen window, and the bug will happen immediately
        ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->Pos);
        ImVec2 viewportSize = viewport->Size;
        ImGui::SetNextWindowSize(viewportSize);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Main window");
        ImGui::PopStyleVar(1);
#endif
    }

    void EndMainWindow()
    {
        ImGui::End();
    }

    void Gui()
    {
        BeginMainWindow();

        ed::Begin("Editor");
        ed::BeginNode(1);
        ImGui::Text("1");
        ed::EndNode();

        // Here, we suspend temporarily the node editor, in order to be able to add another window
        {
            NodeContextHolder::gHolder.Suspend();
            ImGui::Begin("Sus");
            ImGui::Button("Suspended");
            ImGui::End();
           NodeContextHolder::gHolder.Resume();
        }

        ed::End();

        EndMainWindow();
    }

};
