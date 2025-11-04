#include "../../../Headers/Presenter/Commands/InsertCommand.h"

void InsertCommand::setParams(std::string birthNumber, std::string firstName, std::string lastName, std::chrono::year_month_day birthDay)
{
	m_person = new PersonWrapper(new Person(
		birthNumber,
		firstName,
		lastName,
		birthDay
	));
}

void InsertCommand::setParams(unsigned int testId, unsigned int workplaceId, unsigned int districtId, unsigned int regionId, bool result, double testValue,
							  std::string note, std::chrono::time_point<std::chrono::system_clock> testDate, std::string birthNumber)
{
	m_test = new TestByDateWrapper(new PCRTest(
		testId,
		workplaceId,
		districtId,
		regionId,
		result,
		testValue,
		note,
		testDate,
		birthNumber
	), m_database->findPerson(birthNumber));
}

void InsertCommand::execute(std::string& output, std::string& recordCount)
{
	if (m_person != nullptr)
	{
		if (m_database->insert(m_person))
		{
			output = "Person was registered to the database";
			recordCount = "(1) record inserted";
		}
		else
		{
			output = "Insertion failed, person might be already inside";
			recordCount = "(0) records inserted";
		}
	}
	else
	{
		if (m_test->person() == nullptr)
		{
			output = "Insertion failed, person doesn't exist";
			recordCount = "(0) records inserted";
		}
		else if (m_database->insert(m_test))
		{
			output = "Test was registered to the database";
			recordCount = "(1) record inserted";
		}
		else
		{
			output = "Insertion failed, test might be already inside";
			recordCount = "(0) records inserted";
		}
	}
	
	m_person = nullptr;
	m_test = nullptr;
}
