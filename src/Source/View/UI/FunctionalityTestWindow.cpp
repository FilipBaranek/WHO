#include "../../../Headers/View/UI/FunctionalityTestWindow.h"

void FunctionalityTestWindow::handleWindow()
{
    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Always);
    ImGui::Begin("Functionality test");

    ImVec2 regionMin = ImGui::GetWindowContentRegionMin();
    ImVec2 regionMax = ImGui::GetWindowContentRegionMax();
    ImVec2 contentSize = ImVec2(regionMax.x - regionMin.x, regionMax.y - regionMin.y);

    float buttonAreaHeight = 60.0f;
    float margin = 10.0f;
    ImVec2 buttonSize = ImVec2(150, 30);

    ImVec2 childSize = ImVec2(contentSize.x, contentSize.y - buttonAreaHeight - margin);
    ImGui::BeginChild("ScrollableTextRegion", childSize, true, ImGuiWindowFlags_HorizontalScrollbar);

    ImVec2 textSize = ImGui::CalcTextSize(m_output);
    float textX = (childSize.x - textSize.x) * 0.5f;
    if (textX < 0.0f) textX = 0.0f;
    ImGui::SetCursorPosX(textX);

    ImGui::TextUnformatted(m_output);

    ImGui::EndChild();

    ImGui::Dummy(ImVec2(0.0f, margin));

    float buttonX = (contentSize.x - buttonSize.x) * 0.5f;
    ImGui::SetCursorPosX(buttonX);

    if (ImGui::Button("Start", buttonSize))
    {
        m_output = "aaaa\naaaa\naaaa\naaaa\naaaa\naaaa\naaaa\naaaa\naaaa\naaaa\naaaa\naaaa\naaaa\naaaa\naaaa\naaaa\n";
    }

    ImGui::End();
}
