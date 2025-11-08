#pragma once
#include "Window.h"


class GeneratorWindow : public Window
{
private:
	int m_peopleCount = 0;
	int m_testCount = 0;

public:
	GeneratorWindow(Presenter* presenter) : Window(presenter) {}
	void renderWindow() override;
	~GeneratorWindow() = default;
};