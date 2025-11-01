#include "../../../Headers/View/UI/OperationsWindow.h"

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

    static int current_item = 0;
    static const char* items[] = {
        "Select...",
        "2. Vysledok testu podla id testu a id pacienta",
        "3. Vsetky testy pacienta usporiadanych podla datumu",
        "4. Vsetky pozitivne testy za dane casove obdobie pre okres",
        "5. Vsetky testy za dane casove odbobie pre okres",
        "6. Vsetky pozitivne testy za dane casove obdobie pre kraj",
        "7. Vsetky testy za dane casove obdobie pre kraj",
        "20. Odstranit test",
        "21. Odstranit osobu so vsetkymi zaznamami",
        "Vypisat vsetky udaje"
    };

    if (ImGui::BeginCombo("Select Operation", items[current_item]))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            bool is_selected = (current_item == n);
            if (ImGui::Selectable(items[n], is_selected))
            {
                current_item = n;
            }

            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    float buttonWidth = 100.0f;
    float buttonHeight = 30.0f;
    float centerX = (ImGui::GetWindowSize().x - buttonWidth) * 0.5f;
    float bottomY = ImGui::GetWindowSize().y - buttonHeight - 15.0f;

    ImGui::SetCursorPos(ImVec2(centerX, bottomY));

    if (ImGui::Button("Run", ImVec2(buttonWidth, buttonHeight)))
    {
        //
    }

    ImGui::End();
}
