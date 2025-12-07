#pragma once
#include <string>
#include <chrono>
#include "Command.h"
#include "../../View/Models/Person.h"

using namespace std::chrono;

class EditPersonCommand : public Command
{
private:
	std::string m_birthNumber;
	std::string m_firstName;
	std::string m_lastName;
	std::chrono::year_month_day m_birthday;

	PersonView* m_person;

public:
	EditPersonCommand(Database* database) : Command(database), m_person(nullptr) {}

	inline void setParams(std::string birthNumber) { m_birthNumber = birthNumber; }
	
	inline void setParams(std::string birthNumber, std::string firstName, std::string lastName, std::chrono::year_month_day birthday)
	{
		m_birthNumber = birthNumber;
		m_firstName = firstName;
		m_lastName = lastName;
		m_birthday = birthday;
	}

	void execute(std::string& output, std::string& recordCount) override;
	
	void executeEdit(std::string& output, std::string& recordCount);

	inline PersonView* person()
	{
		PersonView* output = m_person;
		m_person = nullptr;
		return output;
	}

	~EditPersonCommand() = default;
};