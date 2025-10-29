#pragma once
#include <string>
#include <sstream>
#include <chrono>
#include <iostream>

class Person
{
private:
	std::string m_birthNumber;
	std::string m_firstName;
	std::string m_lastName;
	std::chrono::year_month_day m_birthDay;
	std::ostringstream m_oss;
public:
	Person(std::string birthNumber, std::string firstName, std::string lastName, std::chrono::year_month_day birthDay) :
		m_birthNumber(birthNumber), m_firstName(firstName), m_lastName(lastName), m_birthDay(birthDay)
	{}
	
	inline std::string birthNumber() { return m_birthNumber; }
	inline std::string firstName() { return m_firstName; }
	inline std::string lastName() { return m_lastName; }
	inline std::chrono::year_month_day birthDay() { return m_birthDay; }

	inline std::string toString()
	{
		m_oss.clear();
		m_oss << "[" << m_birthNumber << "] " << m_firstName << " " << m_lastName << " born " << m_birthDay << "\n";
		return m_oss.str();
	}

	~Person() = default;
};