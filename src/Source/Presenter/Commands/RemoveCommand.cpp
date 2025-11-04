#include "../../../Headers/Presenter/Commands/RemoveCommand.h"

void RemoveCommand::execute(std::string& output, std::string& recordCount)
{
	if (m_removeType == REMOVETYPE::TEST && m_testId >= 0)
	{
		int recordsRemoved = m_database->removeTest(m_testId);
		if (recordsRemoved > 0)
		{
			output = "Test was removed successfuly";
		}
		else
		{
			output = "Test wasn't found";
		}
		recordCount = "(" + std::to_string(recordsRemoved) + ") records deleted";
	}
	else if (m_removeType == REMOVETYPE::PERSON && m_birthNumber != "")
	{
		int recordsRemoved = m_database->removePerson(m_birthNumber);
		if (recordsRemoved > 0)
		{
			output = "Person was removed successfuly";
		}
		else
		{
			output = "Person wasn't found";
		}
		recordCount = "(" + std::to_string(recordsRemoved) + ") records deleted";
	}

	m_birthNumber = "";
	m_testId = -1;
}
