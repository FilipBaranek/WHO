#pragma once
#include <string>
#include "Window.h"

class OutputWindow : public Window
{
private:
	bool m_printPerson;
	std::string m_recordsCount;
	std::string m_output;

public:
	OutputWindow(Presenter* presenter) : Window(presenter) {};
	void renderWindow() override;
	~OutputWindow() = default;
};