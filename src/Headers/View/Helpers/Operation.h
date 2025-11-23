#pragma once
#include <functional>

struct Operation
{
	std::function<void()> display;
	std::function<void()> execute;
};