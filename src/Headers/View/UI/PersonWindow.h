#pragma once
#include <string>
#include <chrono>
#include "Window.h"

class PersonWindow : public Window
{
private:
	std::string m_birthNumber;
	std::string m_firstName;
	std::string m_lastName;
	std::chrono::year_month_day m_birthDay;
	int m_year = 2000;
	int m_month = 1;
	int m_day = 1;

	static constexpr const int CHARACTER_SIZE = 20;
	char birthNumberBuf[CHARACTER_SIZE];
	char firstNameBuf[CHARACTER_SIZE];
	char lastNameBuf[CHARACTER_SIZE];


public:
	PersonWindow(Presenter* presenter) : Window(presenter) {}
	void renderWindow() override;
	~PersonWindow() = default;
};