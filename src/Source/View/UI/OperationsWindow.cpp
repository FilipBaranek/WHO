#include "../../../Headers/View/UI/OperationsWindow.h"

void OperationsWindow::renderWindow()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImVec2 windowSize = ImVec2(viewport->Size.x * 0.25f, viewport->Size.y * 0.41f);
    ImVec2 windowPos = ImVec2(viewport->Pos.x, viewport->Size.y * 0.5f);

    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

    ImGui::Begin("Operations", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings
    );

    ImGui::Columns(3, nullptr, false);

    if (ImGui::Button("2. Test result", ImVec2(-FLT_MIN, 30.0f)))
    {
        //
    }

    ImGui::NextColumn();

    if (ImGui::Button("3. Test by patient", ImVec2(-FLT_MIN, 30.0f)))
    {
        //
    }

    ImGui::NextColumn();

    if (ImGui::Button("Print all data", ImVec2(-FLT_MIN, 30.0f)))
    {
        //
    }

    ImGui::Columns(1);

    ImGui::End();
}
