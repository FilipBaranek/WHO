#include "../../Headers/View/Application.h"

Application::Application(APPLICATIONTYPE type, std::string filePath, int peopleGroupSize,
    int peoplePrimaryClusterSize, int peopleOverflowClusterSize,
    int testsGroupSize, int testsPrimaryClusterSize, int testsOverflowClusterSize)
{
    if (type == APPLICATIONTYPE::RAM)
    {
        m_presenter = new RamPresenter();
        m_windows.push_back(new OperationsWindow(m_presenter));
        m_windows.push_back(new FileWindow(m_presenter));
    }
    else if (type == APPLICATIONTYPE::DISK)
    {
        m_presenter = new DiskPresenter(
            filePath,
            peopleGroupSize,
            peoplePrimaryClusterSize,
            peopleOverflowClusterSize,
            testsGroupSize,
            testsPrimaryClusterSize,
            testsOverflowClusterSize
        );
        m_windows.push_back(new ReducedOperationsWindow(m_presenter));
    }
    m_windows.push_back(new OutputWindow(m_presenter));
    m_windows.push_back(new PersonWindow(m_presenter));
    m_windows.push_back(new TestWindow(m_presenter));
    m_windows.push_back(new GeneratorWindow(m_presenter));
}

void Application::run(bool& continueRun)
{
	for (auto& window : m_windows)
	{
		window->renderWindow();
	}

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 btnSize(100, 40);

    ImGui::SetNextWindowPos(ImVec2(0, io.DisplaySize.y - btnSize.y));

    ImGui::SetNextWindowSize(btnSize);

    ImGui::Begin("ExitOverlay", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNav);

    if (ImGui::Button("EXIT", btnSize))
    {
        continueRun = false;
    }

    ImGui::End();
}

Application::~Application()
{
    for (auto& window : m_windows)
    {
        delete window;
    }

    delete m_presenter;
}
