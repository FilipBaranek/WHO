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
#include "UI/GeneratorWindow.h"
#include "UI/FileWindow.h"
#include "UI/OperationsWindow.h"
#include "UI/ReducedOperationsWindow.h"
#include "../Presenter/RamPresenter.h"
#include "../Presenter/DiskPresenter.h"

enum class APPLICATIONTYPE
{
	RAM,
	DISK
};

class Application
{
private:
	std::vector<Window*> m_windows;
	GLFWwindow* m_mainWindow;
	Presenter* m_presenter;

public:
	Application(APPLICATIONTYPE type);
	void run();
	~Application();
};