#include "../../../Headers/Model/Tests/HashFileTester.h"

HashFileTester::HashFileTester(std::string filePath, int primaryFileClusterSize, int overflowFileClusterSize, int pregeneratedDataCount) :
	m_hashFile(filePath, primaryFileClusterSize, overflowFileClusterSize), seed(m_rd()), m_gen(seed)
{
	m_hashFile.open();

	for (int i{}; i < pregeneratedDataCount; ++i)
	{
		PersonHashWrapper* randomPerson = RandomDataGenerator::generatePerson(m_gen);
		m_data.push_back(randomPerson);
		m_hashFile.insert(randomPerson);
	}
}

void HashFileTester::insert(int operation)
{
	PersonHashWrapper* person = RandomDataGenerator::generatePerson(m_gen);

	m_hashFile.insert(person);
	PersonHashWrapper* foundPerson = m_hashFile.find(person);
	
	if (foundPerson == nullptr || !foundPerson->is(person))
	{
		std::cout << "OPERATION N: " << operation << "\nSEED: " << seed << "\n";
		throw std::runtime_error("Incorrect inserting");
	}

	m_data.push_back(person);

	delete foundPerson;
}

void HashFileTester::find(int operation)
{
	std::uniform_int_distribution<unsigned int> names(0, m_data.size() - 1);

	PersonHashWrapper* person = m_data[names(m_gen)];
	PersonHashWrapper* foundPerson = m_hashFile.find(person);

	if (foundPerson == nullptr || !foundPerson->is(person))
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

	std::cout << seed << "\n\n\n";

	for (int i{}; i < REPLICATIONS; ++i)
	{
		if (i % CHECKPOINT - 1 == 0)
		{
			std::cout << "Operation " << i - 1 << "/" << REPLICATIONS << "\n";
		}

		int operation = probability(m_gen);

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
		}
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
