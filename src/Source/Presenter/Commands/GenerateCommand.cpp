#include "../../../Headers/Presenter/Commands/GenerateCommand.h"

void GenerateCommand::execute(std::string& output, std::string& recordCount)
{
	if (m_peopleCount > 0)
	{
		m_database->generateRandomPeople(m_peopleCount);

		output = "People records were generated";
		recordCount = "(" + std::to_string(m_peopleCount) + ") records were inserted";
	}
	else
	{
		if (m_database->generateRandomTests(m_testCount))
		{
			output = "Tests records were generated";
			recordCount = "(" + std::to_string(m_testCount) + ") records were inserted";
		}
		else
		{
			output = "There are no people in the database";
			recordCount = "(0) records were inserted";
		}
	}

	m_peopleCount = 0;
	m_testCount = 0;
}
