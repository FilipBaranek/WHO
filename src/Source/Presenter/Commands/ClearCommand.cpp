#include "../../../Headers/Presenter/Commands/ClearCommand.h"

void ClearCommand::execute(std::string& output, std::string& recordCount)
{
	m_database->clear();
	output = "Database was completely cleared";
	recordCount = "(" + std::to_string(dynamic_cast<DiskDatabase*>(m_database)->size()) + ") records were deleted";
}
