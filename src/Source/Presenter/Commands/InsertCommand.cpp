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
		if (m_ram)
		{
			if (dynamic_cast<RamDatabase*>(m_database)->insert(new PersonWrapper(m_person)))
			{
				inserted = true;
			}
		}
		else
		{
			dynamic_cast<DiskDatabase*>(m_database)->insert(new PersonHashWrapper(m_person));
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
		if (m_ram)
		{
			if (dynamic_cast<RamDatabase*>(m_database)->insert(new TestWrapper(m_test)))
			{
				inserted = true;
			}
		}
		else
		{
			ReducedPCRTest* test = new ReducedPCRTest(
				m_test->testId(),
				m_test->result(),
				m_test->testValue(),
				m_test->note(),
				m_test->testDate()
			);
			dynamic_cast<DiskDatabase*>(m_database)->insert(new TestHashWrapper(test, m_test->birthNumber()));
			delete m_test;

			inserted = true;
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
	
	m_ram = false;
	m_person = nullptr;
	m_test = nullptr;
}
