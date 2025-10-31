#pragma once
#include "Window.h"

class OperationsWindow : public Window
{
public:
	OperationsWindow(Presenter* presenter) : Window(presenter) {}
	void renderWindow() override;
	~OperationsWindow() override = default;
};