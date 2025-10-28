#include "../../../../Headers/Model/PCRDatabase/Models/Person.h"

Person::Person(std::string birthNumber, std::string firstName, std::string lastName, std::chrono::year_month_day birthDay)
{
	m_birthNumber = birthNumber;
	m_firstName = firstName;
	m_lastName = lastName;
	m_birthDay = birthDay;
}

inline std::string Person::birthNumber()
{
	return m_birthNumber;
}

inline std::string Person::firstName()
{
	return m_firstName;
}

inline std::string Person::lastName()
{
	return m_lastName;
}

inline std::chrono::year_month_day Person::birthDay()
{
	return m_birthDay;
}

inline void Person::print()
{
	std::cout << "[" << m_birthNumber << "] " << m_firstName << " " << m_lastName << " born " << m_birthDay << "\n";
}
