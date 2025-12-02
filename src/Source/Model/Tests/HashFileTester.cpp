#include "../../../Headers/Model/Tests/HashFileTester.h"

HashFileTester::HashFileTester(int primaryFileClusterSize, int overflowFileClusterSize, int pregeneratedDataCount) :
	m_hashFile(primaryFileClusterSize, overflowFileClusterSize), seed(123), m_gen(seed)
{
	m_hashFile.open();

	for (int i{}; i < pregeneratedDataCount; ++i)
	{
		Person* randomPerson = RandomDataGenerator::generatePerson(m_gen);
		m_data.push_back(randomPerson);
		m_hashFile.insert(randomPerson);
	}
}

void HashFileTester::insert(int operation)
{
	Person* person = RandomDataGenerator::generatePerson(m_gen);

	//if (operation == 384)
	//{
	//	Person peter("7312213995", "peter", "Horvat", {});
	//	m_hashFile.find(&peter);
	//}

	m_hashFile.insert(person);

	//if (operation == 384)
	//{
	//	Person peter("7312213995", "peter", "Horvat", {});
	//	m_hashFile.find(&peter);
	//}
	
	//Person* foundPerson = m_hashFile.find(person);
	//if (foundPerson == nullptr || !foundPerson->is(person) || !foundPerson->equals(person))
	//{
	//	std::cout << "OPERATION N: " << operation << "\nSEED: " << seed << "\n";
	//	throw std::runtime_error("Incorrect inserting");
	//}

	//m_data.push_back(person);

	//delete foundPerson;
}

void HashFileTester::find(int operation)
{
	std::uniform_int_distribution<unsigned int> names(0, m_data.size() - 1);

	Person* person = m_data[names(m_gen)];
	Person* foundPerson = m_hashFile.find(person);

	if (foundPerson == nullptr || !foundPerson->is(person) || !foundPerson->equals(person))
	{
		std::cout << "OPERATION N: " << operation << "\nSEED: " << seed << "\n";
		throw std::runtime_error("Incorrect find operation");
	}

	delete foundPerson;
}

void HashFileTester::runTests()
{
	std::uniform_int_distribution<unsigned int> probability(0, 1);
	int inserts = 0, deletes = 0;

	for (int i{}; i < REPLICATIONS; ++i)
	{
		if (i % CHECKPOINT - 1 == 0)
		{
			std::cout << "Operation " << i - 1 << "/" << REPLICATIONS << "\n";
		}

		/*int operation = probability(m_gen);

		if (operation == 0)
		{
			insert(i);
			++inserts;
		}
		else if (operation == 1)
		{
			if (m_data.size() > 0)
			{
				find(i);
			}
		}*/
		//m_hashFile.printOut();

		insert(i);
	}

	m_hashFile.printOut();
}

HashFileTester::~HashFileTester()
{
	for (auto& person : m_data)
	{
		delete person;
	}
}
