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
			delete newTest;
		}
		return true;
	}
}

void DiskDatabase::insert(PersonHashWrapper* person)
{
	m_people.insert(person);
}

void DiskDatabase::insert(TestHashWrapper* test)
{
	m_tests.insert(test);
	delete test;
}

std::pair<std::string, int> DiskDatabase::printAllData()
{
	return std::make_pair(m_people.printOut() + m_tests.printOut(), m_people.size() + m_tests.size());
}

void DiskDatabase::clear()
{
	//
}

DiskDatabase::~DiskDatabase()
{
	RandomDataGenerator::clearGeneratedPeople();
}
