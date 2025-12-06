#include "../../../Headers/Model/PCRDatabase/DiskDatabase.h"

DiskDatabase::DiskDatabase() :
	m_people(PEOPLE_FILE_PATH, PEOPLE_PRIMARY_CLUSTERS_SIZE, PEOPLE_OVERFLOW_CLUSTERS_SIZE),
	m_tests(TESTS_FILE_PATH, TESTS_PRIMARY_CLUSTERS_SIZE, TESTS_OVERFLOW_CLUSTERS_SIZE)
{
	m_people.open();
	m_tests.open();

	if (m_people.size() > 0)
	{
		RandomDataGenerator::initPersonId(m_people.size() + 1);
	}
	if (m_tests.size() > 0)
	{
		RandomDataGenerator::initTestId(m_tests.size() + 1);
	}
}

bool DiskDatabase::updatePersonTests(std::string birthBumber, unsigned int testId)
{
	std::string dummyStr = "";
	Person* dummy = new Person(
		birthBumber,
		dummyStr,
		dummyStr,
		std::chrono::year_month_day{}
	);
	PersonHashWrapper dummyKey(dummy);

	bool updated = m_people.execute(&dummyKey, [&testId](PersonHashWrapper* foundPerson) {
		foundPerson->tests().push_back(testId);
	});

	return updated;
}

void DiskDatabase::generateRandomPeople(int peopleCount)
{
	for (int i{}; i < peopleCount; ++i)
	{
		m_people.insert(RandomDataGenerator::generatePerson(m_gen));
	}
}

bool DiskDatabase::generateRandomTests(int testCount)
{
	if (m_people.size() == 0)
	{
		return false;
	}
	else
	{
		for (int i{}; i < testCount; ++i)
		{
			TestHashWrapper* newTest = RandomDataGenerator::generateTest(m_gen);
			m_tests.insert(newTest);
			updatePersonTests(newTest->person(), newTest->getData()->testId());
			delete newTest;
		}
		return true;
	}
}

//(1)
void DiskDatabase::insert(TestHashWrapper* test)
{
	m_tests.insert(test);
	updatePersonTests(test->person(), test->getData()->testId());

	delete test;
}

//(2)
std::pair<std::string, int> DiskDatabase::findPerson(std::string birthNumber)
{
	int dummyNum = 0;
	std::string dummyStr = "";
	Person* dummyPerson = new Person(
		birthNumber,
		dummyStr,
		dummyStr,
		std::chrono::year_month_day{}
	);
	PersonHashWrapper dummyPersonKey(dummyPerson);

	PersonHashWrapper* foundPerson = m_people.find(&dummyPersonKey);
	std::string strOutput = "";

	if (foundPerson != nullptr)
	{
		strOutput += foundPerson->toString() + "\n\nTests:\n";
		for (auto testId : foundPerson->tests())
		{
			ReducedPCRTest* dummyTest = new ReducedPCRTest(
				testId,
				dummyNum,
				dummyNum,
				dummyStr,
				std::chrono::system_clock::now()
			);
			TestHashWrapper dummyTestKey(dummyTest, birthNumber);

			TestHashWrapper* foundTest = m_tests.find(&dummyTestKey);

			if (foundTest != nullptr)
			{
				strOutput += foundTest->toString();
				delete foundTest;
			}
		}
	}
	else
	{
		strOutput = "Person wasn't found";
	}
	
	auto output = std::make_pair(strOutput, foundPerson->tests().size() + 1);
	
	delete foundPerson;
	return output;
}

//(3)
std::string DiskDatabase::findTest(const unsigned int testId)
{
	int dummyNum = 0;
	std::string dummyStr = "";
	ReducedPCRTest* dummyTest = new ReducedPCRTest(
		testId,
		dummyNum,
		dummyNum,
		dummyStr,
		std::chrono::system_clock::now()
	);
	TestHashWrapper dummyTestKey(dummyTest);

	TestHashWrapper* foundTest = m_tests.find(&dummyTestKey);
	std::string strOutput = "";

	if (foundTest != nullptr)
	{
		Person* dummyPerson = new Person(
			foundTest->person(),
			dummyStr,
			dummyStr,
			std::chrono::year_month_day{}
		);
		PersonHashWrapper dummyPersonKey(dummyPerson);

		PersonHashWrapper* foundPerson = m_people.find(&dummyPersonKey);

		strOutput += foundPerson->toString() + "\n" + foundTest->toString();
	}
	else
	{
		strOutput = "Test wasn't found";
	}

	return strOutput;
}

//(4)
void DiskDatabase::insert(PersonHashWrapper* person)
{
	m_people.insert(person);
}

std::pair<std::string, int> DiskDatabase::printAllData()
{
	return std::make_pair(m_people.printOut() + m_tests.printOut(), m_people.size() + m_tests.size());
}

void DiskDatabase::clear()
{
	m_people.clear();
	m_tests.clear();
}

DiskDatabase::~DiskDatabase()
{
	RandomDataGenerator::clearGeneratedPeople();
}
