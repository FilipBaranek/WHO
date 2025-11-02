#include "../../../Headers/Presenter/Commands/RemoveTestCommand.h"

void RemoveTestCommand::execute(std::string& output, std::string& recordCount)
{
	if (m_testId >= 0)
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
	m_testId = -1;
}
