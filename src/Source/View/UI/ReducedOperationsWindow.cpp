#include "../../../Headers/View/UI/ReducedOperationsWindow.h"

ReducedOperationsWindow::ReducedOperationsWindow(Presenter* presenter) : Window(presenter)
{
    m_operations[0] = {
        []() {},
        []() {}
    };
    m_operations[1] = {
        [this]() {},
        [this]() { printAllData(); }
    };
}

//=========Commands===========
void ReducedOperationsWindow::printAllData()
{
    m_presenter->printAllData();
}

//=========Render=============
void ReducedOperationsWindow::renderWindow()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImVec2 windowSize = ImVec2(viewport->Size.x * 0.25f, viewport->Size.y * 0.5f);
    ImVec2 windowPos = ImVec2(viewport->Pos.x, viewport->Size.y * 0.5f);

    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

    ImGui::Begin("Operations", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings
    );

    static const char* items[] = {
        "Select...",
        "Vypisat vsetky udaje"
    };

    if (ImGui::BeginCombo("Select Operation", items[m_currentItem]))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); ++n)
        {
            bool is_selected = m_currentItem == n;

            if (ImGui::Selectable(items[n], is_selected))
            {
                m_currentItem = n;
            }

            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    if (m_currentItem < m_operations.size())
    {
        m_operations[m_currentItem].display();
    }

    float buttonWidth = 100.0f;
    float buttonHeight = 30.0f;
    float centerX = (ImGui::GetWindowSize().x - buttonWidth) * 0.5f;
    float bottomY = ImGui::GetWindowSize().y - buttonHeight - 15.0f;

    ImGui::SetCursorPos(ImVec2(centerX, bottomY));

    if (ImGui::Button("Run", ImVec2(buttonWidth, buttonHeight)))
    {
        if (m_currentItem < m_operations.size())
        {
            m_operations[m_currentItem].execute();
        }
    }

    ImGui::End();
}
