#include "../../../Headers/Presenter/Commands/InsertCommand.h"

void InsertCommand::setParams(std::string birthNumber, std::string firstName, std::string lastName, std::chrono::year_month_day birthDay)
{
	m_person = new Person(
		birthNumber,
		firstName,
		lastName,
		birthDay
	);
}

void InsertCommand::setParams(unsigned int testId, unsigned int workplaceId, unsigned int districtId, unsigned int regionId, bool result, double testValue,
							  std::string note, std::chrono::time_point<std::chrono::system_clock> testDate, std::string birthNumber)
{
	m_test = new PCRTest(
		testId,
		workplaceId,
		districtId,
		regionId,
		result,
		testValue,
		note,
		testDate,
		birthNumber,
		m_database->findPerson(birthNumber)
	);
}

void InsertCommand::execute()
{
	if (m_person != nullptr)
	{
		m_database->insert(m_person);
	}
	else
	{
		m_database->insert(m_test);
	}

	m_person = nullptr;
	m_test = nullptr;
}
