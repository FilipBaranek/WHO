#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "../Application.h"


class SelectDatabaseWindow
{
private:
	static constexpr const int CHARACTER_SIZE = 20;
	char m_filePathBuff[CHARACTER_SIZE];
	std::string m_filePath;

	int m_peoplePrimaryClusterSize;
	int m_peopleOverflowClusterSize;
	int m_peopleGroupSize;

	int m_testsPrimaryClusterSize;
	int m_testsOverflowClusterSize;
	int m_testsGroupSize;

	bool m_runApp;

	GLFWwindow* m_mainWindow;
	Application* m_app;
	
public:
	SelectDatabaseWindow();
	void displaySelectWindow();
	void run();
	~SelectDatabaseWindow();
};