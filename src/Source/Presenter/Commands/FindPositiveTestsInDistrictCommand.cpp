#include "../../../Headers/Presenter/Commands/FindPositiveTestsInDistrictCommand.h"

void FindPositiveTestsInDistrictCommand::execute(std::string& output, std::string& recordCount)
{
	if (m_districtId >= 0)
	{
		auto result = m_database->findPositiveTestsInDistrict(m_districtId, m_from, m_to);
		output = result.first;
		recordCount = "(" + std::to_string(result.second) + ") records found";
	}
	m_districtId = -1;
}
