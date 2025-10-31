#include "../../Headers/Presenter/Presenter.h"

void Presenter::insert(std::string birthNumber, std::string firstName, std::string lastName, std::chrono::year_month_day birthDay)
{
	m_insertCommand.setParams(
		birthNumber,
		firstName,
		lastName,
		birthDay
	);

	m_insertCommand.execute();
}

void Presenter::insert(unsigned int testId, unsigned int workplaceId, unsigned int districtId, unsigned int regionId, bool result, double testValue,
					   std::string note, std::chrono::time_point<std::chrono::system_clock> testDate, std::string birthNumber)
{
	m_insertCommand.setParams(
		testId,
		workplaceId,
		districtId,
		regionId,
		result,
		testValue,
		note,
		testDate,
		birthNumber
	);

	m_insertCommand.execute();
}

void Presenter::findResultByPatientAndTestId(unsigned int testId, std::string birthNumber, bool printPerson)
{
	m_resultCommand.setParams(testId, birthNumber, printPerson);
	m_resultCommand.execute();
}
