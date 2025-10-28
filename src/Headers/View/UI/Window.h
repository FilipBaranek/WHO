#pragma once

class Window
{
public:
	Window() = default;
	virtual void handleWindow() = 0;
	virtual ~Window() = default;
};