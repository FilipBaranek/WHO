#include "../../../Headers/Presenter/Commands/PrintAllDataCommand.h"

void PrintAllDataCommand::execute(std::string& output, std::string& recordCount)
{
	auto result = m_database->printAllData();
	output = result.first;
	recordCount = "(" + std::to_string(result.second) + ") records found";
}
