#include "../../Headers/Presenter/DiskPresenter.h"


void DiskPresenter::insert(
	unsigned int testId, unsigned int workplaceId, unsigned int districtId, unsigned int regionId,
	bool result, double testValue, std::string note, time_point<system_clock> testDate, std::string birthNumber)
{
}

void DiskPresenter::insert(std::string birthNumber, std::string firstName, std::string lastName, year_month_day birthDay)
{
	execute([=, this](std::string& output, std::string& recordCount) {
		m_insertCommand.setParams(birthNumber, firstName, lastName, birthDay);
		m_insertCommand.execute(output, recordCount);
	});
}

