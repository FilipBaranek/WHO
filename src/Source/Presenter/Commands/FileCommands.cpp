#include "../../../Headers/Presenter/Commands/FileCommands.h"

void FileCommands::execute(std::string& output, std::string& recordCount)
{
	if (m_type == COMMANDTYPE::WRITE)
	{
		auto result = m_database->saveToFile();
		output = result.first;
		recordCount = result.second;
	}
	else if (m_type == COMMANDTYPE::READ)
	{
		auto result = m_database->loadFromFile();
		output = result.first;
		recordCount = result.second;
	}
}
