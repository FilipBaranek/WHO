#include "../../../Headers/Presenter/Commands/FindAllTestsByLocationIdCommand.h"

void FindAllTestsByLocationIdCommand::execute(std::string& output, std::string& recordCount)
{
	std::pair<std::string, int> result;

	if (m_location == LOCATION::DISTRICT)
	{
		result = m_database->findAllTestsInDistrict(m_id, m_from, m_to);
	}
	else if (m_location == LOCATION::REGION)
	{
		result = m_database->findAllTestsInRegion(m_id, m_from, m_to);
	}
	else if (m_location == LOCATION::COUNTRY)
	{
		result = m_database->findAllTests(m_from, m_to);
	}
	else if (m_location == LOCATION::WORKPLACE)
	{
		result = m_database->findAllTestsAtWorkplace(m_id, m_from, m_to);
	}
	output = result.first;
	recordCount = "(" + std::to_string(result.second) + ") records found";
}
