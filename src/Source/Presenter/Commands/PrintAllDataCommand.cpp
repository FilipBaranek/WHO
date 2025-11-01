#include "../../../Headers/Presenter/Commands/PrintAllDataCommand.h"

void PrintAllDataCommand::execute(std::string& output, std::string& recordCount)
{
	output = m_database->printAllData();
}
