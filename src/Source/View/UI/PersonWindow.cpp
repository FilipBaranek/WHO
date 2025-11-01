#include "../../../Headers/View/UI/PersonWindow.h"

void PersonWindow::renderWindow()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImVec2 windowSize = ImVec2(viewport->Size.x * 0.25f, viewport->Size.y * 0.18f);
    ImVec2 windowPos = ImVec2(viewport->Pos.x, viewport->Pos.y);

    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

    ImGui::Begin("Person Information", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings
    );

    strncpy_s(birthNumberBuf, sizeof(birthNumberBuf), m_birthNumber.c_str(), _TRUNCATE);
    strncpy_s(firstNameBuf, sizeof(firstNameBuf), m_firstName.c_str(), _TRUNCATE);
    strncpy_s(lastNameBuf, sizeof(lastNameBuf), m_lastName.c_str(), _TRUNCATE);

    ImGui::Columns(2, nullptr, false);

    ImGui::Text("First Name:");
    ImGui::InputText("##firstName", firstNameBuf, IM_ARRAYSIZE(firstNameBuf));
    ImGui::NextColumn();

    ImGui::Text("Last Name:");
    ImGui::InputText("##lastName", lastNameBuf, IM_ARRAYSIZE(lastNameBuf));
    ImGui::NextColumn();

    ImGui::Text("Birth Number:");
    ImGui::InputText("##birthNumber", birthNumberBuf, IM_ARRAYSIZE(birthNumberBuf));
    ImGui::NextColumn();

    ImGui::Text("Birth Date (Y / M / D):");

    float inputWidth = ImGui::GetColumnWidth() / 4.0f - 5.0f;

    ImGui::PushItemWidth(inputWidth);
    ImGui::InputInt("##year", &m_year, 0, 0);
    ImGui::SameLine();
    ImGui::InputInt("##month", &m_month, 0, 0);
    ImGui::SameLine();
    ImGui::InputInt("##day", &m_day, 0, 0);
    ImGui::PopItemWidth();

    ImGui::Columns(1);

    m_birthNumber = birthNumberBuf;
    m_firstName = firstNameBuf;
    m_lastName = lastNameBuf;
    m_birthDay = std::chrono::year_month_day{
        std::chrono::year{m_year},
        std::chrono::month{static_cast<unsigned>(m_month)},
        std::chrono::day{static_cast<unsigned>(m_day)}
    };

    float buttonWidth = 100.0f;
    float buttonHeight = 30.0f;
    float centerX = (ImGui::GetWindowSize().x - buttonWidth) * 0.5f;
    float bottomY = ImGui::GetWindowSize().y - buttonHeight - 15.0f;

    ImGui::SetCursorPos(ImVec2(centerX, bottomY));

    if (ImGui::Button("Insert", ImVec2(buttonWidth, buttonHeight)))
    {
        if (m_birthNumber != "" && m_lastName != "" && m_lastName != "" &&
            m_year >= 0 && m_month >= 0 && m_day >= 0)
        {
            m_presenter->insert(m_birthNumber, m_firstName, m_lastName, m_birthDay);
        }
        else
        {
            m_presenter->setOutput("Invalid input", "(0) records inserted");
        }
    }

    ImGui::End();
}
