#pragma once
#include <string>
#include <thread>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "Window.h"
#include "../../../Headers/Presenter/Commands/Tests/SpeedTestCommand.h"

class SpeedTestWindow : public Window
{
private:
	bool m_isExecuting;
	bool m_dataInOrder;
	std::string m_output;
	SpeedTestCommand m_command;

public:
	SpeedTestWindow() : m_isExecuting(false), m_output(""), m_command(m_output, m_dataInOrder) {};
	void handleWindow() override;
	~SpeedTestWindow() override = default;
};