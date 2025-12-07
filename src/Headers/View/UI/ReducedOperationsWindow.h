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
private: //PERSON
	bool m_personIsLoaded = false;
	std::string m_birthNumber;
	std::string m_firstName;
	std::string m_lastName;
	std::chrono::year_month_day m_birthDay;
	int m_year = 2000;
	int m_month = 1;
	int m_day = 1;

	static constexpr const int CHARACTER_SIZE = 20;
	char firstNameBuf[CHARACTER_SIZE];
	char lastNameBuf[CHARACTER_SIZE];

private: //TEST
	bool m_testIsLoaded = false;
	int m_testId = 0;
	bool m_result = false;
	double m_testValue = 0;
	int m_hour;
	int m_minute;
	std::string m_note;
	std::chrono::time_point<std::chrono::system_clock> m_testDate;

	char noteBuf[CHARACTER_SIZE];

private:
	static constexpr int OPERATIONS_COUNT = 7;
	std::array<Operation, OPERATIONS_COUNT> m_operations;

	int m_currentItem = 0;

	int m_numBuffer = 0;
	std::string m_stringBuffer;

private:
	void findTest();
	void findPerson();
	void findPersonToEdit();
	void findTestToEdit();
	void printAllData();

private:
	void displayTestInputs();
	void displayPersonInputs();
	void displayPersonEditInputs();
	void displayTestEditInputs();

public:
	ReducedOperationsWindow(Presenter* presenter);
	void renderWindow() override;
	~ReducedOperationsWindow() = default;
};