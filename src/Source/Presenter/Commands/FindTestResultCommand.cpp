#include "../../../Headers/Presenter/Commands/FindTestResultCommand.h"

void FindTestResultCommand::setParams(unsigned int testId, std::string birthNumber, bool printPerson)
{
	m_printPerson = printPerson;
	m_testId = testId;
	m_birthNumber = birthNumber;
}

void FindTestResultCommand::execute()
{
	if (m_testId != -1)
	{
		m_output = m_database->findTestResultByIdAndPatientId(m_testId, m_birthNumber, m_printPerson);
	}
	else
	{
		throw std::runtime_error("Parameteres wasn't set");
	}

	m_printPerson = true;
	m_testId = -1;
	m_birthNumber = "";
}
