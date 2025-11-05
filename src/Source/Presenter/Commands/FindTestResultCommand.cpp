#include "../../../Headers/Presenter/Commands/FindTestResultCommand.h"

void FindTestResultCommand::execute(std::string& output, std::string& recordCount)
{
	if (m_testId != -1)
	{
		output = m_database->findTestResultByIdAndPatientId(m_testId, m_birthNumber);
		recordCount = "(1) record found";
	}
	m_testId = -1;
	m_birthNumber = "";
}
