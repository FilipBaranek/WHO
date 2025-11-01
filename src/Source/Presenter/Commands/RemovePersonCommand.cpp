#include "../../../Headers/Presenter/Commands/RemovePersonCommand.h"


void RemovePersonCommand::execute(std::string& output, std::string& recordCount)
{
	std::pair<bool, int> result = m_database->removePerson(m_birthNumber);

	if (result.first)
	{
		output = "Person records were removed successfuly";
		recordCount = "(" + std::to_string(result.second) + ") records affected";
	}
	else
	{
		output = "Person wasn't found";
		recordCount = "(0) records affected";
	}

	m_birthNumber = "";
}
