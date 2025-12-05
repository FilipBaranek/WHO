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
		birthNumber
	);
}

void InsertCommand::execute(std::string& output, std::string& recordCount)
{
	if (m_person != nullptr)
	{
		bool inserted = false;
		if (m_wrap)
		{
			if (dynamic_cast<RamDatabase*>(m_database)->insert(new PersonWrapper(m_person)))
			{
				inserted = true;
			}
		}
		else
		{
			dynamic_cast<DiskDatabase*>(m_database)->insert(m_person);
			delete m_person;
			inserted = true;
		}
		if (inserted)
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
		bool inserted = false;
		if (m_wrap)
		{
			if (dynamic_cast<RamDatabase*>(m_database)->insert(new TestWrapper(m_test)))
			{
				inserted = true;
			}
		}
		else
		{
			//TODO
		}
		if (inserted)
		{
			output = "Test was registered to the database";
			recordCount = "(1) record inserted";
		}
		else
		{
			output = "Insertion failed, test might be already inside or person doesn't exit";
			recordCount = "(0) records inserted";
		}
	}
	
	m_wrap = false;
	m_person = nullptr;
	m_test = nullptr;
}
