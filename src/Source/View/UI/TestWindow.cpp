#include "../../../Headers/View/UI/TestWindow.h"

void TestWindow::displayRegionCombobox()
{
    ImGui::Text("Region ID:");
    {
        std::string label = (m_selectedRegion == 0) ? "Select Region..." : std::to_string(m_selectedRegion);

        if (ImGui::BeginCombo("##regionCombo", label.c_str()))
        {
            for (int i = 1; i <= MAX_REGION_CODE; ++i)
            {
                bool is_selected = m_selectedRegion == i;
                if (ImGui::Selectable(std::to_string(i).c_str(), is_selected))
                {
                    m_selectedRegion = i;
                    m_regionId = i;
                    m_selectedDistrict = 0;
                    m_selectedWorkplace = 0;
                }
                if (is_selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
    }
    ImGui::NextColumn();
}

void TestWindow::displayDistrictCombobox()
{
    ImGui::Text("District ID:");
    if (m_selectedRegion == 0)
    {
        ImGui::BeginDisabled();
        ImGui::Button("Select Region First", ImVec2(ImGui::GetColumnWidth() - 10, 0));
        ImGui::EndDisabled();
    }
    else
    {
        std::string label = (m_selectedDistrict == 0) ? "Select District..." : std::to_string(m_selectedDistrict);

        if (ImGui::BeginCombo("##districtCombo", label.c_str()))
        {
            unsigned int start = (m_selectedRegion - 1) * 10 + 1;
            unsigned int end = m_selectedRegion * 10;
            for (int i = start; i <= end && i <= MAX_DISTRICT_CODE; ++i)
            {
                bool is_selected = m_selectedDistrict == i;
                if (ImGui::Selectable(std::to_string(i).c_str(), is_selected))
                {
                    m_selectedDistrict = i;
                    m_districtId = i;
                    m_selectedWorkplace = 0;
                }
                if (is_selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
    }
    ImGui::NextColumn();
}

void TestWindow::displayWorkplaceCombobox()
{
    ImGui::Text("Workplace ID:");

    if (m_selectedDistrict == 0)
    {
        ImGui::BeginDisabled();
        ImGui::Button("Select District First", ImVec2(ImGui::GetColumnWidth() - 10, 0));
        ImGui::EndDisabled();
        return;
    }

    std::string label = (m_selectedWorkplace == 0) ? "Select Workplace..." : std::to_string(m_selectedWorkplace);

    if (ImGui::BeginCombo("##workplaceCombo", label.c_str()))
    {
        unsigned int workplacesPerDistrict = 2;
        unsigned int start = (m_selectedDistrict - 1) * workplacesPerDistrict + 1;
        unsigned int end = start + workplacesPerDistrict - 1;

        if (end > MAX_WORKPLACE_CODE)
        {
            end = MAX_WORKPLACE_CODE;
        }

        for (int i = start; i <= end; ++i)
        {
            bool is_selected = m_selectedWorkplace == i;
            if (ImGui::Selectable(std::to_string(i).c_str(), is_selected))
            {
                m_selectedWorkplace = i;
                m_workplaceId = i;
            }
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }
}


void TestWindow::renderWindow()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImVec2 windowSize = ImVec2(viewport->Size.x * 0.25f, viewport->Size.y * 0.2f);
    ImVec2 windowPos = ImVec2(viewport->Pos.x, viewport->Size.y * 0.18f);

    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

    ImGui::Begin("Test Information", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings
    );

    char noteBuf[100];
    strncpy_s(noteBuf, sizeof(noteBuf), m_note.c_str(), _TRUNCATE);

    char birthBuf[20];
    strncpy_s(birthBuf, sizeof(birthBuf), m_birthNumber.c_str(), _TRUNCATE);

    ImGui::Columns(3, nullptr, false);

    // Row 1
    ImGui::Text("Test ID:");
    ImGui::InputInt("##testId", &m_testId, 0, 0);
    ImGui::NextColumn();

    ImGui::Text("Test Value:");
    ImGui::InputDouble("##testValue", &m_testValue, 0, 0, "%.3f");
    ImGui::NextColumn();

    static int year = 2025, month = 1, day = 1, hour = 12, minute = 0;
    ImGui::Text("Res  Hr  Min");
    ImGui::PushItemWidth(ImGui::GetColumnWidth() / 3.5f - 5.0f);
    ImGui::Checkbox("##result", &m_result);
    ImGui::SameLine();
    ImGui::InputInt("##hourh", &hour, 0, 0);
    ImGui::SameLine();
    ImGui::InputInt("##minute", &minute, 0, 0);
    ImGui::PopItemWidth();
    ImGui::NextColumn();

    // Row 2
    ImGui::Text("Note:");
    if (ImGui::InputText("##note", noteBuf, IM_ARRAYSIZE(noteBuf)))
    {
        m_note = noteBuf;
    }
    ImGui::NextColumn();

    ImGui::Text("Birth Number:");
    if (ImGui::InputText("##birthNumber", birthBuf, IM_ARRAYSIZE(birthBuf)))
    {
        m_birthNumber = birthBuf;
	}
    ImGui::NextColumn();

    ImGui::Text("Test Date (Y/M/D):");
    ImGui::PushItemWidth(ImGui::GetColumnWidth() / 3.5f - 5.0f);
    ImGui::InputInt("##year", &year, 0, 0);
    ImGui::SameLine();
    ImGui::InputInt("##month", &month, 0, 0);
    ImGui::SameLine();
    ImGui::InputInt("##day", &day, 0, 0);
    ImGui::PopItemWidth();
    ImGui::NextColumn();

    //Row 3
    displayRegionCombobox();
    displayDistrictCombobox();
    displayWorkplaceCombobox();

    ImGui::Columns(1);

    std::tm tm = {};
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = 0;
    m_testDate = std::chrono::system_clock::from_time_t(std::mktime(&tm));

    float buttonWidth = 100.0f;
    float buttonHeight = 30.0f;
    float centerX = (ImGui::GetWindowSize().x - buttonWidth) * 0.5f;
    float bottomY = ImGui::GetWindowSize().y - buttonHeight - 15.0f;

    ImGui::SetCursorPos(ImVec2(centerX, bottomY));

    if (ImGui::Button("Insert", ImVec2(buttonWidth, buttonHeight)))
    {
        if (m_testId >= 0 && m_workplaceId >= 0 && m_districtId >= 0 && m_regionId >= 0 && m_testValue >= 0 && m_birthNumber != "")
        {
            m_presenter->insert(
                m_testId,
                m_workplaceId,
                m_districtId,
                m_regionId,
                m_result,
                m_testValue,
                m_note,
                m_testDate,
                m_birthNumber
            );
        }
        else
        {
            m_presenter->setOutput("Invalid input", "(0) records inserted");
        }
    }

    ImGui::End();
}
