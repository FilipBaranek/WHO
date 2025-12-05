#include "../../../../Headers/Model/PCRDatabase/Models/Person.h"

std::string Person::toCsvFormat()
{
	std::ostringstream oss;
	oss << m_birthNumber << ";" << m_firstName << ";" << m_lastName << ";"
		<< static_cast<int>(m_birthDay.year()) << "-"
		<< std::setw(2) << std::setfill('0') << static_cast<unsigned>(m_birthDay.month()) << "-"
		<< std::setw(2) << std::setfill('0') << static_cast<unsigned>(m_birthDay.day()) << "\n";

	return oss.str();
}

std::string Person::toString()
{
	std::ostringstream oss;
	oss << "\n[" << m_birthNumber << "] " << m_firstName << " " << m_lastName
		<< " born " << static_cast<int>(m_birthDay.year()) << "-"
		<< std::setw(2) << std::setfill('0') << static_cast<unsigned>(m_birthDay.month()) << "-"
		<< std::setw(2) << std::setfill('0') << static_cast<unsigned>(m_birthDay.day());

	return oss.str();
}