#pragma once
#include <chrono>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "../../Presenter/Presenter.h"

class Window
{
protected:
	Presenter* m_presenter;

public:
	Window(Presenter* presenter) : m_presenter(presenter) {};
	virtual void renderWindow() = 0;
	virtual ~Window() = default;
};