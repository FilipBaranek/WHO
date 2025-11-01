#pragma once
#include <string>
#include <array>
#include "Window.h"
#include "OutputWindow.h"


class OperationsWindow : public Window
{
private:
	static constexpr int OPERATIONS_COUNT = 20;
	std::array<std::function<void()>, OPERATIONS_COUNT> m_operations;

	int m_firstNumInput = 0;
	int m_secondNumInput = 0;
	std::string m_firstStringBuf;
	std::string m_secondStringBuf;

	int m_currentItem = 0;


	void displayTestIdPatientIdInputs();
	void displayPatientIdInput();
	void displayDistrictIdDateInputs();
	void displayRegionIdDateInputs();
	void displayDateInputs();
	void displayDistrictIdDateSickDaysInput();
	void displayRegionDateSickDaysInputs();
	void displayDateSickDaysInputs();
	void displayWorkplaceIdDateInputs();
	void displayTestIdInputs();


public:
	OperationsWindow(Presenter* presenter);
	void renderWindow() override;
	~OperationsWindow() override = default;
};