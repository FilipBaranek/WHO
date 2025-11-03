#include "../../../Headers/Presenter/Commands/FindPositiveTestsByLocationIdCommand.h"

void FindPositiveTestsByLocationIdCommand::execute(std::string& output, std::string& recordCount)
{
	if (m_location == DISTRICT)
	{
		auto result = m_database->findPositiveTestsInDistrict(m_id, m_from, m_to);
		output = result.first;
		recordCount = "(" + std::to_string(result.second) + ") records found";
	}
	else if (m_location == REGION)
	{
		auto result = m_database->findPositiveTestsInRegion(m_id, m_from, m_to);
		output = result.first;
		recordCount = "(" + std::to_string(result.second) + ") records found";
	}
	else if (m_location == COUNTRY)
	{
		auto result = m_database->findPositiveTests(m_from, m_to);
		output = result.first;
		recordCount = "(" + std::to_string(result.second) + ") records found";
	}
	else if (m_location == SICK_IN_DISTRICT)
	{
		auto result = m_database->findSickPeopleInDistrict(m_id, m_from, m_to);
		output = result.first;
		recordCount = "(" + std::to_string(result.second) + ") records found";
	}
}
