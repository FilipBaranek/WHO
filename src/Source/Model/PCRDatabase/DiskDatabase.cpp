#include "../../../Headers/Model/PCRDatabase/DiskDatabase.h"

DiskDatabase::DiskDatabase() : m_people(FILE_PATH, PEOPLE_CLUSTERS_SIZE) //, m_tests(FILE_PATH, TESTS_CLUSTERS_SIZE)
{
	m_people.open();
	//m_tests.open();
}

void DiskDatabase::generateRandomPeople(int peopleCount)
{
	for (int i{}; i < peopleCount; ++i)
	{
		Person* newPerson = RandomDataGenerator::generatePerson(m_gen);
		m_people.insert(newPerson);
		delete newPerson;
	}
}

bool DiskDatabase::generateRandomTests(int testCount)
{
	return false;
}

std::pair<std::string, int> DiskDatabase::printAllData()
{
	//TODO TESTY

	return std::make_pair(m_people.printFile() + m_people.printAddresses(), m_people.size());
}

void DiskDatabase::clear()
{
	m_people.clear();
}
