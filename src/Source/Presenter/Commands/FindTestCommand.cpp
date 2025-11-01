#include "../../../Headers/Presenter/Commands/FindTestCommand.h"

void FindTestCommand::execute(std::string& output, std::string& recordCount)
{
	if (m_testId >= 0)
	{
		output = m_database->findTest(m_testId, true);
		recordCount = "(1) record has been found";
	}

	m_testId = -1;
}
