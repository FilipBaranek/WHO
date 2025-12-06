#include "../../../Headers/Model/Tests/FileTester.h"

FileTester::FileTester(std::string filePath, int clusterSize, int dataCount) : m_heapFile(filePath, clusterSize)
{
	m_heapFile.open();

	for (int i{}; i < dataCount; ++i)
	{
		PersonHashWrapper* person = RandomDataGenerator::generatePerson(m_gen);

		int address = m_heapFile.insert(person);
		m_data[person] = address;
	}
}

void FileTester::insert()
{
	PersonHashWrapper* person = RandomDataGenerator::generatePerson(m_gen);

	int address = m_heapFile.insert(person);
	PersonHashWrapper* foundPerson = m_heapFile.find(address, person);

	if (!person->is(foundPerson))
	{
		throw std::runtime_error("Incorrect insertion");
	}

	m_data[person] = address;
	delete foundPerson;
}

void FileTester::find()
{
	std::uniform_int_distribution<unsigned int> randomAddressInterval(0, m_data.size() - 1);

	auto iterator = m_data.begin();
	std::advance(iterator, randomAddressInterval(m_gen));

	int address = iterator->second;
	PersonHashWrapper* personToFind = iterator->first;
	PersonHashWrapper* foundPerson = m_heapFile.find(address, personToFind);

	if (foundPerson == nullptr || !personToFind->is(foundPerson))
	{
		throw std::runtime_error("Incorrect find operation");
	}
	delete foundPerson;
}

void FileTester::remove()
{
	std::uniform_int_distribution<unsigned int> randomAddressInterval(0, m_data.size() - 1);

	auto iterator = m_data.begin();
	std::advance(iterator, randomAddressInterval(m_gen));

	int address = iterator->second;
	PersonHashWrapper* personToRemove = iterator->first;
	PersonHashWrapper* removedPerson = m_heapFile.remove(address, personToRemove);
	PersonHashWrapper* foundPerson = m_heapFile.find(address, personToRemove);

	if (removedPerson == nullptr || !personToRemove->is(removedPerson) || foundPerson != nullptr)
	{
		throw std::runtime_error("Incorrect removal");
	}

	m_data.erase(personToRemove);
	delete personToRemove;
	delete removedPerson;
	delete foundPerson;
}

void FileTester::printOut(int& inserts, int& deletes)
{
	std::cout << m_heapFile.printFile();
	std::cout << m_heapFile.printAddresses();
	std::cout << "\n\nFile size: " << m_heapFile.size() << "\n";
	std::cout << "Inserts: " << inserts << " Deletes: " << deletes << "\n\n";
}

void FileTester::runTests()
{
	std::uniform_int_distribution<unsigned int> probability(0, 2);
	int inserts = 0, deletes = 0;

	for (int i{}; i < REPLICATIONS; ++i)
	{
		if (i % CHECKPOINT - 1 == 0)
		{
			std::cout << "Operation " << i - 1 << "/" << REPLICATIONS << "\n";
			//testSize();
		}

		int operation = probability(m_gen);

		switch (operation)
		{
		case 0:
			insert();
			++inserts;
			break;
		case 1:
			if (m_data.size() > 0)
			{
				find();
			}
			break;
		case 2:
		default:
			if (m_data.size() > 0)
			{
				remove();
				++deletes;
			}
			break;
		}
	}
	testSize();
	printOut(inserts, deletes);
}

FileTester::~FileTester()
{
	for (auto& [person, _] : m_data)
	{
		delete person;
	}
}
