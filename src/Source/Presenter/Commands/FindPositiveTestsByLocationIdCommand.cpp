#include "../../../Headers/Presenter/Commands/FindPositiveTestsByLocationIdCommand.h"

void FindPositiveTestsByLocationIdCommand::execute(std::string& output, std::string& recordCount)
{
	std::pair<std::string, int> result;
	
	if (m_searchType == SEARCHTYPE::BY_DATE)
	{
		if (m_location == LOCATION::DISTRICT)
		{
			result = dynamic_cast<RamDatabase*>(m_database)->findPositiveTestsInDistrict(m_id, m_from, m_to);
		}
		else if (m_location == LOCATION::REGION)
		{
			result = dynamic_cast<RamDatabase*>(m_database)->findPositiveTestsInRegion(m_id, m_from, m_to);
		}
		else if (m_location == LOCATION::COUNTRY)
		{
			result = dynamic_cast<RamDatabase*>(m_database)->findPositiveTests(m_from, m_to);
		}
	}
	else if (m_searchType == SEARCHTYPE::SICK_PEOPLE)
	{
		if (m_location == LOCATION::DISTRICT && !m_orderBy)
		{
			result = dynamic_cast<RamDatabase*>(m_database)->findSickPeopleInDistrict(m_id, m_from, m_to);
		}
		else if (m_location == LOCATION::DISTRICT && m_orderBy)
		{
			result = dynamic_cast<RamDatabase*>(m_database)->findSickPeopleInDistrictOrderedByTestValue(m_id, m_from, m_to);
		}
		else if (m_location == LOCATION::REGION)
		{
			result = dynamic_cast<RamDatabase*>(m_database)->findSickPeopleInRegion(m_id, m_from, m_to);
		}
		else if (m_location == LOCATION::COUNTRY)
		{
			result = dynamic_cast<RamDatabase*>(m_database)->findSickPeople(m_from, m_to);
		}
	}
	else if (m_searchType == SEARCHTYPE::ORDERED_BY_SICK_PEOPLE)
	{
		if (m_location == LOCATION::DISTRICT)
		{
			result = dynamic_cast<RamDatabase*>(m_database)->findDistrictsOrderedBySickPeopleCount(m_from, m_to);
		}
		else if (m_location == LOCATION::REGION)
		{
			result = dynamic_cast<RamDatabase*>(m_database)->findRegionsOrderedBySickPeopleCount(m_from, m_to);
		}
	}
	else if (m_searchType == SEARCHTYPE::MOST_SICK_PERSON)
	{
		if (m_location == LOCATION::DISTRICT)
		{
			result = dynamic_cast<RamDatabase*>(m_database)->findMostSickPersonInDistrict(m_from, m_to);
		}
	}

	output = result.first;
	recordCount = "(" + std::to_string(result.second) + ") records found";
}
