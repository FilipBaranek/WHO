#include "../../../Headers/Presenter/Commands/FindPersonCommand.h"

void FindPersonCommand::execute(std::string& output, std::string& recordCount)
{
	if (m_birthNumber != "")
	{
		auto result = dynamic_cast<DiskDatabase*>(m_database)->findPerson(m_birthNumber);
		output = result.first;
		recordCount = "(" + std::to_string(result.second) + ") record has been found";
	}

	m_birthNumber = "";
}
