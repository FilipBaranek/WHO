#include "../../../Headers/Presenter/Commands/FindAllTestsByLocationIdCommand.h"

void FindAllTestsByLocationIdCommand::execute(std::string& output, std::string& recordCount)
{
	if (m_location == DISTRICT)
	{
		auto result = m_database->findAllTestsInDistrict(m_id, m_from, m_to);
		output = result.first;
		recordCount = "(" + std::to_string(result.second) + ") records found";
	}
	else if (m_location == REGION)
	{
		auto result = m_database->findAllTestsInRegion(m_id, m_from, m_to);
		output = result.first;
		recordCount = "(" + std::to_string(result.second) + ") records found";
	}
	else if (m_location == COUNTRY)
	{
		auto result = m_database->findAllTests(m_from, m_to);
		output = result.first;
		recordCount = "(" + std::to_string(result.second) + ") records found";
	}
}
