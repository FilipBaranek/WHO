#pragma once
#include <vector>
#include "UI/Window.h"
#include "UI/OutputWindow.h"
#include "UI/PersonWindow.h"
#include "UI/TestWindow.h"
#include "UI/GeneratorWindow.h"
#include "UI/FileWindow.h"
#include "UI/OperationsWindow.h"
#include "UI/ReducedOperationsWindow.h"
#include "../Presenter/RamPresenter.h"
#include "../Presenter/DiskPresenter.h"

enum class APPLICATIONTYPE
{
	RAM,
	DISK
};

class Application
{
private:
	std::vector<Window*> m_windows;
	Presenter* m_presenter;

public:
	Application(APPLICATIONTYPE type, std::string filePath, int peopleGroupSize, int peoplePrimaryClusterSize,
		int peopleOverflowClusterSize, int testsGroupSize, int testsPrimaryClusterSize, int testsOverflowClusterSize);
	
	void run(bool& continueRun);
	
	~Application();
};