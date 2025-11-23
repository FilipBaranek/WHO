#include "../../../Headers/Presenter/Commands/FindTestResultCommand.h"

void FindTestResultCommand::execute(std::string& output, std::string& recordCount)
{
	if (m_testId != -1)
	{
		auto result = dynamic_cast<RamDatabase*>(m_database)->findTestResultByIdAndPatientId(m_testId, m_birthNumber);
		output = result.first;
		recordCount = "(" + std::to_string(result.second) + ") record found";
	}
	m_testId = -1;
	m_birthNumber = "";
}
