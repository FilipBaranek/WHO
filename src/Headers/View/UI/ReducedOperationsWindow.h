#pragma once
#include <string>
#include <array>
#include <utility>
#include "Window.h"
#include "OutputWindow.h"
#include "../../Presenter/DiskPresenter.h"
#include "../Helpers/Operation.h"

class ReducedOperationsWindow : public Window
{
private:
	static constexpr int OPERATIONS_COUNT = 7;
	std::array<Operation, OPERATIONS_COUNT> m_operations;

	int m_currentItem = 0;

	int m_numBuffer = 0;
	std::string m_stringBuffer;

private:
	void findTest();
	void findPerson();
	void printAllData();

private:
	void displayTestInputs();
	void displayPersonInputs();

public:
	ReducedOperationsWindow(Presenter* presenter);
	void renderWindow() override;
	~ReducedOperationsWindow() = default;
};