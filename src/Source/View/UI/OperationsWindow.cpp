#include "../../../Headers/View/UI/OperationsWindow.h"


OperationsWindow::OperationsWindow(Presenter* presenter) : Window(presenter)
{
    m_operations[0] = {
        []() {},
        []() {}
    };
    m_operations[1] = {
        [this]() { displayTestIdPatientIdInputs(); },
        [this]() { findTestResultByPatientId(); }
    };
    m_operations[2] = {
        [this]() { displayPatientIdInput(); },
        [this]() { findPatientsSortedTests(); }
    };
    m_operations[3] = {
        [this]() { displayNumIdDateInputs(); },
        [this]() { findPositiveTestsInDateRangeForDistrict(); }
    };
    m_operations[4] = {
        [this]() { displayNumIdDateInputs(); },
        [this]() { findAllTestsInDateRangeForDistrict(); }
    };
    m_operations[5] = {
        [this]() { displayNumIdDateInputs(); },
        [this]() { findPositiveTestsInDateRangeForRegion(); }
    };
    m_operations[6] = {
        [this]() { displayNumIdDateInputs(); },
        [this]() { findAllTestsInDateRangeForRegion(); }
    };
    m_operations[7] = {
        [this]() { displayDateInputs(); },
        [this]() { findPositiveTestsInDateRange(); }
    };
    m_operations[8] = {
        [this]() { displayDateInputs(); },
        [this]() { findAllTestsInDateRange(); }
    };
    m_operations[9] = {
        [this]() { displayNumIdDateSickDaysInput(); },
        [this]() { findSickPeopleInDistrictByDate(); }
    };
    m_operations[10] = {
        [this]() { displayNumIdDateSickDaysInput(); },
        [this]() { findSickPeopleInDistrictByDateSorted(); }
    };
    m_operations[11] = {
        [this]() { displayNumIdDateSickDaysInput(); },
        [this]() { findSickPeopleInRegionByDate(); }
    };
    m_operations[12] = {
        [this]() { displayDateSickDaysInputs(); },
        [this]() { findSickPeopleByDate(); }
    };
    m_operations[13] = {
        [this]() { displayDateSickDaysInputs(); },
        [this]() { findMostSickPersonInEveryDistrict(); }
    };
    m_operations[14] = {
        [this]() { displayDateSickDaysInputs(); },
        [this]() { printAllDistrictsOrderedBySickPeopleCount(); }
    };
    m_operations[15] = {
        [this]() { displayDateSickDaysInputs(); },
        [this]() { printAllRegionsOrderedBySickPeopleCount(); }
    };
    m_operations[16] = {
        [this]() { displayNumIdDateInputs(); },
        [this]() { findAllTestsDoneAtWorkplaceByDate(); }
    };
    m_operations[17] = {
        [this]() { displayTestIdInputs(); },
        [this]() { findTest(); }
    };
    m_operations[18] = {
        [this]() { displayTestIdInputs(); },
        [this]() { removeTest(); }
    };
    m_operations[19] = {
        [this]() { displayPatientIdInput(); },
        [this]() { removePerson(); }
    };
    m_operations[20] = {
        []() {},
        [this]() { printAllData(); }
    };
}

//=================DISPLAY===========================
void OperationsWindow::displayTestIdPatientIdInputs()
{
    ImGui::Dummy(ImVec2(20.0f, 60.0f));

    char birthBuf[20];
    strncpy_s(birthBuf, sizeof(birthBuf), m_firstStringBuf.c_str(), _TRUNCATE);

    float windowHeight = ImGui::GetWindowSize().y;
    float rowHeight = ImGui::GetTextLineHeight() + ImGui::GetStyle().FramePadding.y * 2; // height of one input row
    ImGui::SetCursorPosY((windowHeight - rowHeight) * 0.5f);

    ImGui::Columns(2, nullptr, false);

    ImGui::Text("Test ID:");
    ImGui::InputInt("##testId", &m_firstNumInput, 0, 0);

    ImGui::NextColumn();

    ImGui::Text("Birth Number:");
    if (ImGui::InputText("##birthNumber", birthBuf, IM_ARRAYSIZE(birthBuf)))
    {
        m_firstStringBuf = birthBuf;
    }

    ImGui::Columns(1);
}

void OperationsWindow::displayPatientIdInput()
{
    ImGui::Dummy(ImVec2(20.0f, 70.0f));

    char birthBuf[20];
    strncpy_s(birthBuf, sizeof(birthBuf), m_firstStringBuf.c_str(), _TRUNCATE);

    ImGui::Text("Birth Number:");
    if (ImGui::InputText("##birthNumber", birthBuf, IM_ARRAYSIZE(birthBuf)))
    {
        m_firstStringBuf = birthBuf;
    }
}

void OperationsWindow::displayNumIdDateInputs()
{
    ImGui::Dummy(ImVec2(20.0f, 50.0f));

    ImGui::Text("ID:");
    ImGui::InputInt("##numId", &m_firstNumInput, 0, 0);

    displayDateInputs();
}

void OperationsWindow::displayDateInputs()
{
    ImGui::Dummy(ImVec2(20.0f, 50.0f));

    static int yearFrom = 2025, monthFrom = 1, dayFrom = 1;
    ImGui::Text("Period from (Y/M/D):");
    ImGui::PushItemWidth(ImGui::GetColumnWidth() / 3.5f - 5.0f);
    ImGui::InputInt("##yearFrom", &yearFrom, 0, 0);
    ImGui::SameLine();
    ImGui::InputInt("##monthFrom", &monthFrom, 0, 0);
    ImGui::SameLine();
    ImGui::InputInt("##dayFrom", &dayFrom, 0, 0);
    ImGui::PopItemWidth();

    static int yearTo = 2025, monthTo = 1, dayTo = 1;
    ImGui::Text("Period to (Y/M/D):");
    ImGui::PushItemWidth(ImGui::GetColumnWidth() / 3.5f - 5.0f);
    ImGui::InputInt("##yearTo", &yearTo, 0, 0);
    ImGui::SameLine();
    ImGui::InputInt("##monthTo", &monthTo, 0, 0);
    ImGui::SameLine();
    ImGui::InputInt("##dayTo", &dayTo, 0, 0);
    ImGui::PopItemWidth();
}

void OperationsWindow::displaySickDaysInput()
{
    ImGui::Text("Sick days:");
    ImGui::InputInt("##sickdaysId", &m_secondNumInput, 0, 0);
}

void OperationsWindow::displayNumIdDateSickDaysInput()
{
    displayNumIdDateInputs();
    displaySickDaysInput();
}

void OperationsWindow::displayDateSickDaysInputs()
{
    displayDateInputs();
    displaySickDaysInput();
}

void OperationsWindow::displayTestIdInputs()
{
    ImGui::Dummy(ImVec2(20.0f, 80.0f));

    ImGui::Text("Test ID:");
    ImGui::InputInt("##testId", &m_firstNumInput, 0, 0);
}

//===============REQUESTS==============================
void OperationsWindow::printAllData()
{
    m_presenter->printAllData();
}

void OperationsWindow::findTestResultByPatientId()
{
    m_presenter->findResultByPatientAndTestId(m_firstNumInput, m_firstStringBuf, true);
}

void OperationsWindow::findPatientsSortedTests()
{
}

void OperationsWindow::findPositiveTestsInDateRangeForDistrict()
{
}

void OperationsWindow::findAllTestsInDateRangeForDistrict()
{

}

void OperationsWindow::findPositiveTestsInDateRangeForRegion()
{

}

void OperationsWindow::findAllTestsInDateRangeForRegion()
{
}

void OperationsWindow::findPositiveTestsInDateRange()
{
}

void OperationsWindow::findAllTestsInDateRange()
{

}

void OperationsWindow::findSickPeopleInDistrictByDate()
{
}

void OperationsWindow::findSickPeopleInDistrictByDateSorted()
{
}

void OperationsWindow::findSickPeopleInRegionByDate()
{
}

void OperationsWindow::findSickPeopleByDate()
{
}

void OperationsWindow::findMostSickPersonInEveryDistrict()
{
}

void OperationsWindow::printAllDistrictsOrderedBySickPeopleCount()
{
}

void OperationsWindow::printAllRegionsOrderedBySickPeopleCount()
{
}

void OperationsWindow::findAllTestsDoneAtWorkplaceByDate()
{
}

void OperationsWindow::findTest()
{
    m_presenter->findTest(m_firstNumInput);
}

void OperationsWindow::removeTest()
{
    m_presenter->removeTest(m_firstNumInput);
}

void OperationsWindow::removePerson()
{
    m_presenter->removePerson(m_firstStringBuf);
}


//=================RENDER===============================
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

    static const char* items[] = {
        "Select...",
        "2. Vysledok testu podla id testu a id pacienta",
        "3. Vsetky testy pacienta usporiadanych podla datumu",
        "4. Vsetky pozitivne testy za dane casove obdobie pre okres",
        "5. Vsetky testy za dane casove odbobie pre okres",
        "6. Vsetky pozitivne testy za dane casove obdobie pre kraj",
        "7. Vsetky testy za dane casove obdobie pre kraj",
        "8. Vsetky pozitivne testy za dane casove obdobie",
        "9. Testy za dane casove obdobie",
        "10. Chore osoby v okrese k zadanemu datumu",
        "11. Usporiadane chore osoby k zadanemu datumu",
        "12. Chore osoby v kraji k zadanemu datumu",
        "13. Chore osoby k zadanemu datumu",
        "14. Chora osoba k zadanemu datumu s najvyssou hodnotou testu v okrese",
        "15. Okresy usporiadane podla poctu chorych osob",
        "16. Kraje usporiadane podla poctu chorych osob",
        "17. Testy uskutocnene za dane casove obdobie na zadanom pracovisku",
        "18. Najdi test",
        "20. Odstranit test",
        "21. Odstranit osobu so vsetkymi zaznamami",
        "Vypisat vsetky udaje"
    };

    if (ImGui::BeginCombo("Select Operation", items[m_currentItem]))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
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
