#include "../../../Headers/Presenter/Commands/FindTestResultCommand.h"

void FindTestResultCommand::setParams(unsigned int testId, std::string birthNumber, bool printPerson)
{
	m_printPerson = printPerson;
	m_testId = testId;
	m_birthNumber = birthNumber;
}

void FindTestResultCommand::execute(std::string& output, std::string& recordCount)
{
	if (m_testId != -1)
	{
		output = m_database->findTestResultByIdAndPatientId(m_testId, m_birthNumber, m_printPerson);
		recordCount = "(1) record found";
	}

	m_printPerson = true;
	m_testId = -1;
	m_birthNumber = "";
}
