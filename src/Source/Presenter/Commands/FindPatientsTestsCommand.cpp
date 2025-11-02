#include "../../../Headers/Presenter/Commands/FindPatientsTestsCommand.h"

void FindPatientsTestsCommand::execute(std::string& output, std::string& recordCount)
{
	if (m_birthNumber != "")
	{
		auto result = m_database->findPatientTestOrderByDate(m_birthNumber);
		output = result.first;
		recordCount = "(" + std::to_string(result.second) + ") records found";
	}

	m_birthNumber = "";
}
