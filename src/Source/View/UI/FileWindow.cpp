#include "../../../Headers/View/UI/FileWindow.h"

void FileWindow::renderWindow()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImVec2 windowSize = ImVec2(viewport->Size.x * 0.25f, viewport->Size.y * 0.1f);
    ImVec2 windowPos = ImVec2(viewport->Pos.x, viewport->Size.y - (viewport->Size.y * 0.1f));

    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

    ImGui::Begin("File", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings
    );

    ImGui::Columns(2, nullptr, false);

    if (ImGui::Button("Save", ImVec2(-FLT_MIN, 30.0f)))
    {
        //
    }
    ImGui::NextColumn();

    if (ImGui::Button("Load", ImVec2(-FLT_MIN, 30.0f)))
    {
        //
    }

    ImGui::Columns(1);

    ImGui::End();
}
