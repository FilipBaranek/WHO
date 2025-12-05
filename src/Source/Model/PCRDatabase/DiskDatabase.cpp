#include "../../../Headers/Model/PCRDatabase/DiskDatabase.h"

DiskDatabase::DiskDatabase() //:
	//m_people(PEOPLE_FILE_PATH, PEOPLE_PRIMARY_CLUSTERS_SIZE, PEOPLE_OVERFLOW_CLUSTERS_SIZE),
	//m_tests(TESTS_FILE_PATH, TESTS_PRIMARY_CLUSTERS_SIZE, TESTS_OVERFLOW_CLUSTERS_SIZE)
{
	//m_people.open();
	//m_tests.open();
}

void DiskDatabase::generateRandomPeople(int peopleCount)
{
	/*for (int i{}; i < peopleCount; ++i)
	{
		Person* newPerson = RandomDataGenerator::generatePerson(m_gen);
		m_people.insert(newPerson);
		delete newPerson;
	}*/
}

bool DiskDatabase::generateRandomTests(int testCount)
{
	return false;
}

void DiskDatabase::insert(Person* person)
{
	//m_people.insert(person);
}

std::pair<std::string, int> DiskDatabase::printAllData()
{
	//TODO TESTY

	//return std::make_pair(m_people.printOut(), m_people.size());
	return std::make_pair("", 0);
}

void DiskDatabase::clear()
{
	//m_people.clear();
}
