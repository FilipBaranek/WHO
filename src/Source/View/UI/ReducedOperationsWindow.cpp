#include "../../../Headers/View/UI/ReducedOperationsWindow.h"

ReducedOperationsWindow::ReducedOperationsWindow(Presenter* presenter) : Window(presenter)
{
    m_operations[0] = {
        []() {},
        []() {}
    };
    m_operations[1] = {
        [this]() { displayPersonInputs(); },
        [this]() { findPerson(); }
    };
    m_operations[2] = {
        [this]() { displayTestInputs(); },
        [this]() { findTest(); }
    };
    m_operations[3] = {
        [this]() { displayPersonInputs(); },
        [this]() { findPersonToEdit(); }
    };
    m_operations[4] = {
        [this]() { displayTestInputs(); },
        [this]() { findTestToEdit(); }
    };
    m_operations[5] = {
        []() {},
        [this]() { printAllData(); }
    };
    m_operations[6] = {
        []() {},
        [this]() { clearDatabase(); }
    };
}

//=========Commands===========
void ReducedOperationsWindow::printAllData()
{
    m_presenter->printAllData();
}

void ReducedOperationsWindow::clearDatabase()
{
    dynamic_cast<DiskPresenter*>(m_presenter)->clear();
}

void ReducedOperationsWindow::findTest()
{
    m_presenter->findTest(m_numBuffer);
}

void ReducedOperationsWindow::findPerson()
{
    dynamic_cast<DiskPresenter*>(m_presenter)->findPerson(m_stringBuffer);
}

void ReducedOperationsWindow::findPersonToEdit()
{
    dynamic_cast<DiskPresenter*>(m_presenter)->findPersonToEdit(m_stringBuffer);
}

void ReducedOperationsWindow::findTestToEdit()
{
    dynamic_cast<DiskPresenter*>(m_presenter)->findTestToEdit(m_numBuffer);
}

//=========Render=============
void ReducedOperationsWindow::displayTestInputs()
{
    ImGui::Dummy(ImVec2(20.0f, 60.0f));

    ImGui::Text("Test ID:");
    ImGui::InputInt("##testId", &m_numBuffer, 0, 0);
}

void ReducedOperationsWindow::displayPersonInputs()
{
    ImGui::Dummy(ImVec2(20.0f, 60.0f));

    char birthBuf[20];
    strncpy_s(birthBuf, sizeof(birthBuf), m_stringBuffer.c_str(), _TRUNCATE);

    ImGui::Text("Birth Number:");
    if (ImGui::InputText("##birthNumber", birthBuf, IM_ARRAYSIZE(birthBuf)))
    {
        m_stringBuffer = birthBuf;
    }
}

void ReducedOperationsWindow::displayPersonEditInputs()
{
    strncpy_s(firstNameBuf, sizeof(firstNameBuf), m_firstName.c_str(), _TRUNCATE);
    strncpy_s(lastNameBuf, sizeof(lastNameBuf), m_lastName.c_str(), _TRUNCATE);

    ImGui::Columns(2, nullptr, false);

    ImGui::Text("First Name:");
    ImGui::InputText("##firstName", firstNameBuf, IM_ARRAYSIZE(firstNameBuf));
    ImGui::NextColumn();

    ImGui::Text("Last Name:");
    ImGui::InputText("##lastName", lastNameBuf, IM_ARRAYSIZE(lastNameBuf));
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

    if (ImGui::Button("Edit", ImVec2(buttonWidth, buttonHeight)))
    {
        if (m_firstName != "" && m_lastName != "" && m_year >= 0 && m_month >= 0 && m_day >= 0)
        {
            dynamic_cast<DiskPresenter*>(m_presenter)->editPerson(m_birthNumber, m_firstName, m_lastName, m_birthDay);
        }
    }
}

void ReducedOperationsWindow::displayTestEditInputs()
{
    char noteBuf[100];
    strncpy_s(noteBuf, sizeof(noteBuf), m_note.c_str(), _TRUNCATE);

    char birthBuf[20];
    strncpy_s(birthBuf, sizeof(birthBuf), m_birthNumber.c_str(), _TRUNCATE);

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

    ImGui::Text("Test Date (Y/M/D):");
    ImGui::PushItemWidth(ImGui::GetColumnWidth() / 3.5f - 5.0f);
    ImGui::InputInt("##year", &year, 0, 0);
    ImGui::SameLine();
    ImGui::InputInt("##month", &month, 0, 0);
    ImGui::SameLine();
    ImGui::InputInt("##day", &day, 0, 0);
    ImGui::PopItemWidth();
    ImGui::NextColumn();

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

    if (ImGui::Button("Edit", ImVec2(buttonWidth, buttonHeight)))
    {
        if (m_testId >= 0 && m_testValue >= 0)
        {
            dynamic_cast<DiskPresenter*>(m_presenter)->editTest(m_testId, m_result, m_testValue, m_note, m_testDate);
        }
    }
}

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

    static bool showRunButton = true;
    static const char* items[] = {
        "Select...",
        "(2) Najst osobu",
        "(3) Najst test",
        "(7) Editovat osobu",
        "(8) Editovat test",
        "Vypisat vsetky udaje",
        "Vymazat databazu"
    };

    if (ImGui::BeginCombo("Select Operation", items[m_currentItem]))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); ++n)
        {
            bool is_selected = m_currentItem == n;

            if (ImGui::Selectable(items[n], is_selected))
            {
                m_currentItem = n;

                showRunButton = true;
                m_personIsLoaded = false;
                m_testIsLoaded = false;
            }

            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    if (showRunButton && m_currentItem < m_operations.size())
    {
        m_operations[m_currentItem].display();
    }

    float buttonWidth = 100.0f;
    float buttonHeight = 30.0f;
    float centerX = (ImGui::GetWindowSize().x - buttonWidth) * 0.5f;
    float bottomY = ImGui::GetWindowSize().y - buttonHeight - 15.0f;

    if (showRunButton)
    {
        ImGui::SetCursorPos(ImVec2(centerX, bottomY));

        if (ImGui::Button("Run", ImVec2(buttonWidth, buttonHeight)))
        {
            if (m_currentItem < m_operations.size())
            {
                m_operations[m_currentItem].execute();
            }
        }
    }

    if (!m_presenter->isExecuting())
    {
        if (m_currentItem == 3 && m_personIsLoaded)
        {
            displayPersonEditInputs(); 
        }
        else if (m_currentItem == 3 && !m_personIsLoaded)
        {
            auto person = dynamic_cast<DiskPresenter*>(m_presenter)->person();
            if (person != nullptr)
            {
                m_personIsLoaded = true;
                m_birthNumber = person->birthNumber();
                m_firstName = person->firstName();
                m_lastName = person->lastName();
                m_year = static_cast<int>(person->birthDay().year());
                m_month = static_cast<unsigned int>(person->birthDay().month());
                m_day = static_cast<unsigned int>(person->birthDay().day());
                showRunButton = false;
                dynamic_cast<DiskPresenter*>(m_presenter)->removePerson();
            }
        }
        else if (m_currentItem == 4 && m_testIsLoaded)
        {
            displayTestEditInputs();
        }
        else if (m_currentItem == 4 && !m_testIsLoaded)
        {
            auto test = dynamic_cast<DiskPresenter*>(m_presenter)->test();
            if (test != nullptr)
            {
                m_testIsLoaded = true;
                m_testId = test->testId();
                m_result = test->result();
                m_testValue = test->testValue();
                m_note = test->note();

                auto dp = floor<days>(test->testDate());
                year_month_day ymd = year_month_day{ dp };
                hh_mm_ss hms{ test->testDate() - dp};
                m_year = int(ymd.year());
                m_month = unsigned(ymd.month());
                m_day = unsigned(ymd.day());
                m_hour = hms.hours().count();
                m_minute = hms.minutes().count();

                showRunButton = false;
                dynamic_cast<DiskPresenter*>(m_presenter)->removeTest();
            }
        }
    }

    ImGui::End();
}
