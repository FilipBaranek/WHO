#pragma once
#include <string>
#include <chrono>

class PersonView
{
private:
	std::string m_birthNumber;
	std::string m_firstName;
	std::string m_lastName;
	std::chrono::year_month_day m_birthDay;

public:
	PersonView(std::string birthNumber, std::string firstName, std::string lastName, std::chrono::year_month_day birthDay) :
		m_birthNumber(birthNumber), m_firstName(firstName), m_lastName(lastName), m_birthDay(birthDay)
	{}

	inline std::string birthNumber() { return m_birthNumber; }

	inline std::string firstName() { return m_firstName; }

	inline std::string lastName() { return m_lastName; }

	inline std::chrono::year_month_day birthDay() { return m_birthDay; }

	~PersonView() = default;
};