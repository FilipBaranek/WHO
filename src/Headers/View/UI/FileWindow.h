#pragma once
#include "Window.h"

class FileWindow : public Window
{
public:
	FileWindow(Presenter* presenter) : Window(presenter) {}
	void renderWindow() override;
	~FileWindow() = default;
};