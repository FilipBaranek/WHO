#include "../../../Headers/Presenter/Commands/FindAllTestsByLocationIdCommand.h"

void FindAllTestsByLocationIdCommand::execute(std::string& output, std::string& recordCount)
{
	std::pair<std::string, int> result;

	if (m_location == LOCATION::DISTRICT)
	{
		result = dynamic_cast<RamDatabase*>(m_database)->findAllTestsInDistrict(m_id, m_from, m_to);
	}
	else if (m_location == LOCATION::REGION)
	{
		result = dynamic_cast<RamDatabase*>(m_database)->findAllTestsInRegion(m_id, m_from, m_to);
	}
	else if (m_location == LOCATION::COUNTRY)
	{
		result = dynamic_cast<RamDatabase*>(m_database)->findAllTests(m_from, m_to);
	}
	else if (m_location == LOCATION::WORKPLACE)
	{
		result = dynamic_cast<RamDatabase*>(m_database)->findAllTestsAtWorkplace(m_id, m_from, m_to);
	}
	output = result.first;
	recordCount = "(" + std::to_string(result.second) + ") records found";
}
