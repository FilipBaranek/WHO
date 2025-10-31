#include "../../../Headers/View/UI/GeneratorWindow.h"

void GeneratorWindow::renderWindow()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImVec2 windowSize = ImVec2(viewport->Size.x * 0.25f, viewport->Size.y * 0.12f);
    ImVec2 windowPos = ImVec2(viewport->Pos.x, viewport->Size.y * 0.38f);

    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

    ImGui::Begin("Generator", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings
    );

    // --- 2x2 grid layout ---
    ImGui::Columns(2, nullptr, false);

    // Row 1, Column 1
    ImGui::Text("People Count:");
    ImGui::InputInt("##peopleCount", &m_peopleCount);
    ImGui::NextColumn();

    // Row 1, Column 2
    ImGui::Text("Test Count:");
    ImGui::InputInt("##testCount", &m_testCount);
    ImGui::NextColumn();

    // Row 2, Column 1
    if (ImGui::Button("Generate People", ImVec2(-FLT_MIN, 30.0f)))
    {
        //
    }
    ImGui::NextColumn();

    // Row 2, Column 2
    if (ImGui::Button("Generate Tests", ImVec2(-FLT_MIN, 30.0f)))
    {
        m_peopleCount = 0;
        m_testCount = 0;
    }

    ImGui::Columns(1);

    ImGui::End();
}
