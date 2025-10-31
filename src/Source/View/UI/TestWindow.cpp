#include "../../../Headers/View/UI/TestWindow.h"

void TestWindow::renderWindow()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImVec2 windowSize = ImVec2(viewport->Size.x * 0.25f, viewport->Size.y * 0.20f);
    ImVec2 windowPos = ImVec2(viewport->Pos.x, viewport->Size.y * 0.18f);

    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

    ImGui::Begin("Test Information", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings
    );

    char noteBuf[128];
    strncpy_s(noteBuf, sizeof(noteBuf), m_note.c_str(), _TRUNCATE);

    char birthBuf[32];
    strncpy_s(birthBuf, sizeof(birthBuf), m_birthNumber.c_str(), _TRUNCATE);

    ImGui::Columns(3, nullptr, false);

    // Row 1
    ImGui::Text("Test ID:");
    ImGui::InputInt("##testId", &m_testId, 0, 0);
    ImGui::NextColumn();

    ImGui::Text("Workplace ID:");
    ImGui::InputInt("##workplaceId", &m_workplaceId, 0, 0);
    ImGui::NextColumn();

    ImGui::Text("District ID:");
    ImGui::InputInt("##districtId", &m_districtId, 0, 0);
    ImGui::NextColumn();

    // Row 2
    ImGui::Text("Region ID:");
    ImGui::InputInt("##regionId", &m_regionId, 0, 0);
    ImGui::NextColumn();

    ImGui::Text("Test Value:");
    ImGui::InputDouble("##testValue", &m_testValue, 0, 0, "%.3f");
    ImGui::NextColumn();

    // Row 3
    ImGui::Text("Note:");
    if (ImGui::InputText("##note", noteBuf, IM_ARRAYSIZE(noteBuf)))
        m_note = noteBuf;
    ImGui::NextColumn();

    ImGui::Text("Birth Number:");
    if (ImGui::InputText("##birthNumber", birthBuf, IM_ARRAYSIZE(birthBuf)))
        m_birthNumber = birthBuf;
    ImGui::NextColumn();

    ImGui::Text("Test Date (Y/M/D):");
    static int year = 2025, month = 1, day = 1;
    ImGui::PushItemWidth(ImGui::GetColumnWidth() / 3.5f - 5.0f);
    ImGui::InputInt("##year", &year, 0, 0);
    ImGui::SameLine();
    ImGui::InputInt("##month", &month, 0, 0);
    ImGui::SameLine();
    ImGui::InputInt("##day", &day, 0, 0);
    ImGui::PopItemWidth();
    ImGui::NextColumn();

    ImGui::Text("Result:");
    ImGui::Checkbox("##result", &m_result);
    ImGui::NextColumn();

    ImGui::Columns(1);

    std::tm tm = {};
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    m_testDate = std::chrono::system_clock::from_time_t(std::mktime(&tm));

    float buttonWidth = 100.0f;
    float buttonHeight = 30.0f;
    float totalButtonWidth = buttonWidth * 2 + 20.0f;
    float centerX = (ImGui::GetWindowSize().x - totalButtonWidth) * 0.5f;
    float bottomY = ImGui::GetWindowSize().y - buttonHeight - 15.0f;

    ImGui::SetCursorPos(ImVec2(centerX, bottomY));

    if (ImGui::Button("Insert", ImVec2(buttonWidth, buttonHeight)))
    {
        //
    }

    ImGui::SameLine(0, 20.0f);

    if (ImGui::Button("Delete", ImVec2(buttonWidth, buttonHeight)))
    {
        //
    }

    ImGui::End();
}
