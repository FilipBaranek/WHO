#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <vector>
#include "../../Headers/View/UI/Window.h"

class Application
{
private:
	std::vector<Window*> m_windows;
	GLFWwindow* m_mainWindow;

public:
	Application();
	void run();
	~Application();
};