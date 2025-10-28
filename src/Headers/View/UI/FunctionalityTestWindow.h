#pragma once
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "Window.h"

class FunctionalityTestWindow : public Window
{
private:
	const char* m_output = "";

public:
	FunctionalityTestWindow() = default;
	void handleWindow() override;
	~FunctionalityTestWindow() override = default;
};