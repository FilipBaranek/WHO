#include "../../../Headers/View/UI/SelectDatabase.h"

SelectDatabaseWindow::SelectDatabaseWindow()
{
    m_runApp = false;
    m_peopleGroupSize = 0;
    m_testsGroupSize = 0;
    m_peoplePrimaryClusterSize = 0;
    m_peopleOverflowClusterSize = 0;
    m_testsPrimaryClusterSize = 0;
    m_testsOverflowClusterSize = 0;
    m_app = nullptr;

    glfwInit();
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    m_mainWindow = glfwCreateWindow(1920, 1080, "WHO", nullptr, nullptr);
    glfwMakeContextCurrent(m_mainWindow);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_mainWindow, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void SelectDatabaseWindow::displaySelectWindow()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImVec2 windowSize = ImVec2(viewport->Size.x * 0.25f, viewport->Size.y * 0.2f);
    ImVec2 windowPos = ImVec2(viewport->Pos.x * 0.5, viewport->Pos.y * 0.5);

    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

    ImGui::Begin("Person Information", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings
    );

    strncpy_s(m_filePathBuff, sizeof(m_filePathBuff), m_filePath.c_str(), _TRUNCATE);

    ImGui::Columns(2, nullptr, false);

    ImGui::Text("File name:");
    if (ImGui::InputText("##fileName", m_filePathBuff, IM_ARRAYSIZE(m_filePathBuff)))
    {
        m_filePath = m_filePathBuff;
    }
    ImGui::NextColumn();

    ImGui::Text("People group size:");
    ImGui::InputInt("##peopleGrouSize", &m_peopleGroupSize, 0, 0);
    ImGui::NextColumn();

    ImGui::Text("People primary cluster size:");
    ImGui::InputInt("##peoplePrimaryClusterSize", &m_peoplePrimaryClusterSize, 0, 0);
    ImGui::NextColumn();

    ImGui::Text("People overflow cluster size:");
    ImGui::InputInt("##peopleOverflowClusterSize", &m_peopleOverflowClusterSize, 0, 0);
    ImGui::NextColumn();

    ImGui::Text("Tests group size:");
    ImGui::InputInt("##testsGrouSize", &m_testsGroupSize, 0, 0);
    ImGui::NextColumn();

    ImGui::Text("Tests primary cluster size:");
    ImGui::InputInt("##testsPrimaryClusterSize", &m_testsPrimaryClusterSize, 0, 0);
    ImGui::NextColumn();

    ImGui::Text("Tests overflow cluster size:");
    ImGui::InputInt("##testsOverflowClusterSize", &m_testsOverflowClusterSize, 0, 0);
    ImGui::NextColumn();

    float buttonWidth = -10.0f;
    float buttonHeight = 30.0f;
    float centerX = (ImGui::GetWindowSize().x - buttonWidth) * 0.5f;
    float bottomY = ImGui::GetWindowSize().y - buttonHeight - 15.0f;

    ImGui::SetCursorPos(ImVec2(centerX, bottomY));

    if (ImGui::Button("Run", ImVec2(buttonWidth, buttonHeight)))
    {
        m_runApp = true;
    }

    ImGui::End();
}

void SelectDatabaseWindow::run()
{
    while (!glfwWindowShouldClose(m_mainWindow)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (!m_runApp)
        {
            if (m_app != nullptr)
            {
                delete m_app;
                m_app = nullptr;
            }
            displaySelectWindow();
        }
        else
        {
            if (m_app == nullptr)
            {
                m_app = new Application(
                    APPLICATIONTYPE::DISK,
                    m_filePath,
                    m_peopleGroupSize,
                    m_peoplePrimaryClusterSize,
                    m_peopleOverflowClusterSize,
                    m_testsGroupSize,
                    m_testsPrimaryClusterSize,
                    m_testsOverflowClusterSize
                );
            }
            m_app->run(m_runApp);
        }

        ImGui::Render();
        glViewport(0, 0, 800, 600);
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_mainWindow);
    }
}

SelectDatabaseWindow::~SelectDatabaseWindow()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(m_mainWindow);
    glfwTerminate();

    delete m_app;
}