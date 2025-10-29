#pragma once
#include <thread>
#include <imgui.h>
#include <string>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "Window.h"
#include "../../Presenter/Commands/Tests/FunctionalityTestCommand.h"

class FunctionalityTestWindow : public Window
{
private:
	bool m_isExecuting = false;
	unsigned int m_userInputDataCount;
	std::string m_output = "";
	FunctionalityTestCommand m_command;

public:
	FunctionalityTestWindow() : m_command(m_userInputDataCount, m_output), m_userInputDataCount(0) {};
	void handleWindow() override;
	~FunctionalityTestWindow() override = default;
};