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
        []() {},
        []() {}
    };
    m_operations[4] = {
        []() {},
        []() {}
    };
    m_operations[5] = {
        [this]() {},
        [this]() { printAllData(); }
    };
    m_operations[6] = {
        []() {},
        []() {}
    };
}

//=========Commands===========
void ReducedOperationsWindow::printAllData()
{
    m_presenter->printAllData();
}

void ReducedOperationsWindow::findTest()
{
    m_presenter->findTest(m_numBuffer);
}

void ReducedOperationsWindow::findPerson()
{
    dynamic_cast<DiskPresenter*>(m_presenter)->findPerson(m_stringBuffer);
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
