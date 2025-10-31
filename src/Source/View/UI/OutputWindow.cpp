#include "../../../Headers/View/UI/OutputWindow.h"

void OutputWindow::renderWindow()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImVec2 windowSize = ImVec2(viewport->Size.x * 0.75f, viewport->Size.y);
    ImVec2 windowPos = ImVec2(
        viewport->Pos.x + viewport->Size.x - windowSize.x,
        viewport->Pos.y
    );

    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

    ImGui::Begin("Output", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoSavedSettings
    );

    ImGui::BeginChild("MainOutput", ImVec2(0, -40), true, ImGuiWindowFlags_HorizontalScrollbar);

    ImGui::PushFont(ImGui::GetFont());
    ImGui::SetWindowFontScale(1.5f);

    ImVec2 textSize = ImGui::CalcTextSize(m_output.c_str());
    float textX = (ImGui::GetWindowSize().x - textSize.x) * 0.5f;
    if (textX < 0) textX = 0;

    ImGui::SetCursorPosX(textX);
    ImGui::TextUnformatted(m_output.c_str());

    ImGui::PopFont();
    ImGui::EndChild();

    float footerHeight = 30.0f;
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - footerHeight);

    ImGui::SetCursorPosX(10);
    ImGui::Checkbox("Include person information", &m_printPerson);

    ImVec2 recordSize = ImGui::CalcTextSize(m_recordsCount.c_str());
    float recordX = ImGui::GetWindowSize().x - recordSize.x - 20.0f;

    ImGui::SetCursorPos(ImVec2(recordX, ImGui::GetWindowHeight() - footerHeight));
    ImGui::TextUnformatted(m_recordsCount.c_str());

    ImGui::End();


}
