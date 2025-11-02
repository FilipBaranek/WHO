#pragma once
#include <string>
#include <sstream>
#include <chrono>
#include <iostream>
#include "../ModelWrappers/TestWrapper.h"
#include "../../Structures/AVL/AVLTree.h"

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
	
	inline void setBirthNumber(std::string birthNumber) { m_birthNumber = birthNumber; }
	inline std::string birthNumber() { return m_birthNumber; }
	inline std::string firstName() { return m_firstName; }
	inline std::string lastName() { return m_lastName; }
	inline std::chrono::year_month_day birthDay() { return m_birthDay; }

	inline std::string toString()
	{
		m_oss.str("");
		m_oss.clear();
		m_oss << "\n[" << m_birthNumber << "] " << m_firstName << " " << m_lastName
			  << " born " << static_cast<int>(m_birthDay.year()) << "-"
		      << std::setw(2) << std::setfill('0') << static_cast<unsigned>(m_birthDay.month()) << "-"
			  << std::setw(2) << std::setfill('0') << static_cast<unsigned>(m_birthDay.day());
		
		return m_oss.str();
	}

	~Person() = default;
};