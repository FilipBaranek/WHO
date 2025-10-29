#include "../../../Headers/View/UI/FunctionalityTestWindow.h"


void FunctionalityTestWindow::handleWindow()
{
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Always);
    ImGui::Begin("Functionality Test");

    ImVec2 contentSize = ImGui::GetContentRegionAvail();

    float margin = 10.0f;
    float inputWidth = 100.0f;
    float buttonWidth = 150.0f;
    float buttonHeight = 30.0f;

    ImVec2 childSize = ImVec2(contentSize.x, contentSize.y * 0.75f);
    ImGui::BeginChild("ScrollableTextRegion", childSize, true, ImGuiWindowFlags_HorizontalScrollbar);

    ImVec2 textSize = ImGui::CalcTextSize(m_output.c_str());
    float textX = (childSize.x - textSize.x) * 0.5f;
    if (textX < 0.0f) textX = 0.0f;
    ImGui::SetCursorPosX(textX);
    ImGui::TextUnformatted(m_output.c_str());

    ImGui::EndChild();
    ImGui::Dummy(ImVec2(0.0f, margin));

    float centerX = (contentSize.x - inputWidth) * 0.5f;
    ImGui::SetCursorPosX(centerX);

    if (!m_isExecuting)
    {
        ImGui::PushItemWidth(inputWidth);
        ImGui::InputScalar("Pregen data size", ImGuiDataType_U32, &m_userInputDataCount, nullptr, nullptr, "%u");
        ImGui::PopItemWidth();

        ImGui::Dummy(ImVec2(0.0f, margin));

        ImGui::SetCursorPosX((contentSize.x - buttonWidth) * 0.5f);
        if (ImGui::Button("Start Test", ImVec2(buttonWidth, buttonHeight)))
        {
            m_isExecuting = true;

            std::thread([this]() {
                m_command.execute();
                m_isExecuting = false;
            }).detach();
        }
    }
    else
    {
        ImVec2 textSize = ImGui::CalcTextSize("Testing...");
        ImGui::SetCursorPosX((contentSize.x - textSize.x) * 0.5f);
        ImGui::Text("Testing...");
    }

    ImGui::End();
}
