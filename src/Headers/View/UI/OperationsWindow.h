#pragma once
#include <string>
#include <array>
#include "Window.h"
#include "OutputWindow.h"


struct Operation
{
	std::function<void()> display;
	std::function<void()> execute;
};

class OperationsWindow : public Window
{
private:
	static constexpr int OPERATIONS_COUNT = 21;
	std::array<Operation, OPERATIONS_COUNT> m_operations;

	int m_firstNumInput = 0;
	int m_secondNumInput = 0;
	std::string m_firstStringBuf;
	std::string m_secondStringBuf;

	int m_currentItem = 0;


	void displayTestIdPatientIdInputs();
	void displayPatientIdInput();
	void displayNumIdDateInputs();
	void displayDateInputs();
	void displaySickDaysInput();
	void displayNumIdDateSickDaysInput();
	void displayDateSickDaysInputs();
	void displayTestIdInputs();

	void printAllData();
	void findTestResultByPatientId();
	void findPatientsSortedTests();
	void findPositiveTestsInDateRangeForDistrict();
	void findAllTestsInDateRangeForDistrict();
	void findPositiveTestsInDateRangeForRegion();
	void findAllTestsInDateRangeForRegion();
	void findPositiveTestsInDateRange();
	void findAllTestsInDateRange();
	void findSickPeopleInDistrictByDate();
	void findSickPeopleInDistrictByDateSorted();
	void findSickPeopleInRegionByDate();
	void findSickPeopleByDate();
	void findMostSickPersonInEveryDistrict();
	void printAllDistrictsOrderedBySickPeopleCount();
	void printAllRegionsOrderedBySickPeopleCount();
	void findAllTestsDoneAtWorkplaceByDate();
	void findTest();
	void removeTest();
	void removePerson();

public:
	OperationsWindow(Presenter* presenter);
	void renderWindow() override;
	~OperationsWindow() override = default;
};