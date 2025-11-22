#include "../../../Headers/Model/Tests/FileTester.h"

FileTester::FileTester(std::string filePath, int clusterSize, int dataCount) : m_heapFile(filePath, clusterSize)
{
	m_heapFile.open();

	for (int i{}; i < dataCount; ++i)
	{
		Person* person = RandomDataGenerator::generatePerson(m_gen);

		int address = m_heapFile.insert(person);
		m_data[person] = address;
	}
}

void FileTester::insert()
{
	Person* person = RandomDataGenerator::generatePerson(m_gen);

	int address = m_heapFile.insert(person);
	Person* foundPerson = m_heapFile.find(address, person);

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
	Person* personToFind = iterator->first;
	Person* foundPerson = m_heapFile.find(address, personToFind);

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
	Person* personToRemove = iterator->first;
	Person* removedPerson = m_heapFile.remove(address, personToRemove);
	Person* foundPerson = m_heapFile.find(address, personToRemove);

	if (removedPerson == nullptr || !personToRemove->is(removedPerson) || foundPerson != nullptr)
	{
		throw std::runtime_error("Incorrect removal");
	}

	m_data.erase(personToRemove);
	delete personToRemove;
	delete removedPerson;
	delete foundPerson;
}

void FileTester::duplicityTest()
{
	Person* person = RandomDataGenerator::generatePerson(m_gen);
	Person* equalPerson = person;
	Person* duplicitPerson = new Person(
		person->birthNumber(),
		"bb",
		"aa",
		person->birthDay()
	);

	int addr1 = m_heapFile.insert(person);
	int addr2 = m_heapFile.insert(equalPerson);
	int addr3 = m_heapFile.insert(duplicitPerson);

	Person* foundEqualPerson = m_heapFile.find(addr2, equalPerson);
	Person* foundDuplicitPerson = m_heapFile.find(addr3, duplicitPerson);
	if (!foundEqualPerson->equals(person) || !foundDuplicitPerson->is(person))
	{
		throw std::runtime_error("Incorrect duplicit find operation");
	}
	delete foundEqualPerson;
	delete foundDuplicitPerson;

	delete m_heapFile.remove(addr1, person);
	foundEqualPerson = m_heapFile.find(addr2, person);
	if (!foundEqualPerson->equals(person))
	{
		throw std::runtime_error("Incorrect duplicit remove");
	}
	delete foundEqualPerson;

	foundDuplicitPerson = m_heapFile.find(addr3, person);
	if (!foundDuplicitPerson->is(person))
	{
		throw std::runtime_error("Incorrect duplicit remove");
	}
	delete foundDuplicitPerson;

	delete m_heapFile.remove(addr2, person);
	delete m_heapFile.remove(addr3, person);
	Person* foundPerson = m_heapFile.find(addr1, person);
	foundEqualPerson = m_heapFile.find(addr2, person);
	foundDuplicitPerson = m_heapFile.find(addr3, duplicitPerson);
	if (foundPerson != nullptr || foundEqualPerson != nullptr || foundDuplicitPerson != nullptr)
	{
		throw std::runtime_error("Incorrect duplicit remove");
	}

	delete foundPerson;
	delete foundEqualPerson;
	delete foundDuplicitPerson;
	delete person;
	delete duplicitPerson;
}

void FileTester::printOut(int& inserts, int& deletes)
{
	m_heapFile.printFile();
	m_heapFile.printAddresses();
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

			duplicityTest();
			testSize();
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
