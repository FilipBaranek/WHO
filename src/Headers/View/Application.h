#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <vector>
#include "UI/Window.h"
#include "UI/OutputWindow.h"
#include "UI/PersonWindow.h"
#include "UI/TestWindow.h"
#include "../Presenter/Presenter.h"

class Application
{
private:
	std::vector<Window*> m_windows;
	GLFWwindow* m_mainWindow;
	Presenter* m_presenter;

public:
	Application();
	void run();
	~Application();
};