#include "../../Headers/View/Application.h"

//Application::Application()
//{
//	glfwInit();
//    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
//	m_mainWindow = glfwCreateWindow(1920, 1080, "WHO", nullptr, nullptr);
//    glfwMakeContextCurrent(m_mainWindow);
//    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
//
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO(); (void)io;
//    ImGui::StyleColorsDark();
//
//    ImGui_ImplGlfw_InitForOpenGL(m_mainWindow, true);
//    ImGui_ImplOpenGL3_Init("#version 130");
//
//    m_windows.push_back(new FunctionalityTestWindow());
//    m_windows.push_back(new SpeedTestWindow());
//}
//
//void Application::run()
//{
//    while (!glfwWindowShouldClose(m_mainWindow)) {
//        glfwPollEvents();
//
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplGlfw_NewFrame();
//        ImGui::NewFrame();
//
//        for (auto& window : m_windows)
//        {
//            window->handleWindow();
//        }
//
//        ImGui::Render();
//        glViewport(0, 0, 800, 600);
//        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//        glfwSwapBuffers(m_mainWindow);
//    }
//}
//
//Application::~Application()
//{
//    for (auto& window : m_windows)
//    {
//        delete window;
//    }
//
//    ImGui_ImplOpenGL3_Shutdown();
//    ImGui_ImplGlfw_Shutdown();
//    ImGui::DestroyContext();
//    glfwDestroyWindow(m_mainWindow);
//    glfwTerminate();
//}
