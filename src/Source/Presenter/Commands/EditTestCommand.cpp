#include "../../../Headers/Presenter/Commands/EditTestCommand.h"

void EditTestCommand::execute(std::string& output, std::string& recordCount)
{
	if (m_testId != -1)
	{
		auto result = dynamic_cast<DiskDatabase*>(m_database)->findTestToEdit(m_testId);

		m_test = result.first;
		output = result.second;
		recordCount = result.first != nullptr ? "(1) record was found" : "(0) records were found";
	}
	m_testId = -1;
}

void EditTestCommand::executeEdit(std::string& output, std::string& recordCount)
{
	if (m_testId != -1)
	{
		ReducedPCRTest* test = new ReducedPCRTest(
			m_testId,
			m_result,
			m_testValue,
			m_note,
			m_testDate
		);

		bool result = dynamic_cast<DiskDatabase*>(m_database)->editTest(test);

		if (result)
		{
			output = "Test was successfuly edited";
			recordCount = 1;
		}
		else
		{
			output = "Test wasn't edited";
			recordCount = "(0) records were affected";
		}
	}
	m_testId = -1;
}