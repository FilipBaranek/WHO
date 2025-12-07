#include "../../../Headers/Presenter/Commands/EditPersonCommand.h"

void EditPersonCommand::execute(std::string& output, std::string& recordCount)
{
	if (m_birthNumber != "")
	{
		auto result = dynamic_cast<DiskDatabase*>(m_database)->findPersonToEdit(m_birthNumber);

		m_person = result.first;
		output = result.second;
		recordCount = result.first != nullptr ? "(1) record was found" : "(0) records were found";
	}
	m_birthNumber = "";
}

void EditPersonCommand::executeEdit(std::string& output, std::string& recordCount)
{
	if (m_birthNumber != "")
	{
		Person* person = new Person(
			m_birthNumber,
			m_firstName,
			m_lastName,
			m_birthday
		);

		bool result = dynamic_cast<DiskDatabase*>(m_database)->editPerson(person);

		if (result)
		{
			output = "Person was successfuly edited";
			recordCount = 1;
		}
		else
		{
			output = "Person wasn't edited";
			recordCount = "(0) records were affected";
		}
	}
	m_birthNumber = "";
}
