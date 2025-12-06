#include "../../../Headers/Model/PCRDatabase/DiskDatabase.h"

DiskDatabase::DiskDatabase() :
	m_people(PEOPLE_FILE_PATH, PEOPLE_PRIMARY_CLUSTERS_SIZE, PEOPLE_OVERFLOW_CLUSTERS_SIZE),
	m_tests(TESTS_FILE_PATH, TESTS_PRIMARY_CLUSTERS_SIZE, TESTS_OVERFLOW_CLUSTERS_SIZE)
{
	m_people.open();
	m_tests.open();
}

void DiskDatabase::generateRandomPeople(int peopleCount)
{
	for (int i{}; i < peopleCount; ++i)
	{
		PersonHashWrapper* newPerson = RandomDataGenerator::generatePerson(m_gen);
		m_peopleList.push_back(newPerson);
		m_people.insert(newPerson);
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
			TestHashWrapper* newTest = RandomDataGenerator::generateTest(m_gen, m_peopleList);
			m_tests.insert(newTest);
			delete newTest;
		}
	}
}

void DiskDatabase::insert(PersonHashWrapper* person)
{
	m_people.insert(person);
	delete person;
}

void DiskDatabase::insert(TestHashWrapper* test)
{
	m_tests.insert(test);
	delete test;
}

std::pair<std::string, int> DiskDatabase::printAllData()
{
	//NEED TO REFACTOR

	return std::make_pair(m_people.printOut() + m_tests.printOut(), m_people.size() + m_tests.size());
}

void DiskDatabase::clear()
{
	for (auto& person : m_peopleList)
	{
		delete person;
	}

	m_people.close();
	m_tests.close();
}
