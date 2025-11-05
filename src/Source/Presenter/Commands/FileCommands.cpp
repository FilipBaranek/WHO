#include "../../../Headers/Presenter/Commands/FileCommands.h"

void FileCommands::execute(std::string& output, std::string& recordCount)
{
	std::pair<std::string, int> result;
	if (m_type == COMMANDTYPE::WRITE)
	{
		result = m_database->saveToFile();
		recordCount = "(" + std::to_string(result.second) + ") records were saved";
	}
	else if (m_type == COMMANDTYPE::READ)
	{
		result = m_database->loadFromFile();

		recordCount = "(" + std::to_string(result.second) + ") records were loaded";
	}
	output = result.first;
}
