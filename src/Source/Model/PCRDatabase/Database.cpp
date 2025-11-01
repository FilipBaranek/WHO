#include "../../../Headers/Model/PCRDatabase/Database.h"


Database::Database()
{
	m_testStructuresList.push_back(&m_tests);
}

void Database::generateRandomPeople(int peopleCount)
{
	for (int i{}; i < peopleCount; ++i)
	{
		RandomDataGenerator::generatePeople(m_peopleList, m_people);
	}
}

bool Database::generateRandomTests(int testCount)
{
	if (m_people.size() > 0)
	{
		for (int i{}; i < testCount; ++i)
		{
			RandomDataGenerator::generateTests(m_peopleList, m_testStructuresList);
		}
		return true;
	}
	return false;
}

bool Database::insert(Person* person)
{
	PersonWrapper* personWrapper = new PersonWrapper(person);
	if (m_people.insert(personWrapper))
	{
		return true;
	}

	delete personWrapper;
	return false;
}

bool Database::insert(PCRTest* pcrTest)
{
	for (auto& testStructure : m_testStructuresList)
	{
		TestWrapper* test = new TestWrapper(pcrTest);
		if (!testStructure->insert(test))
		{
			delete test;
			return false;
		}
	}
	return true;
}

Person* Database::findPerson(std::string birthNumber)
{
	Person person(
		birthNumber,
		DEFAULT_STRING_VAL,
		DEFAULT_STRING_VAL,
		DEFAULT_DATE
	);
	PersonWrapper key(&person);

	PersonWrapper* foundPerson = m_people.find(&key);
	return foundPerson != nullptr ? foundPerson->getData() : nullptr;
}

std::string Database::findTestResultByIdAndPatientId(const unsigned int testId, const std::string birthBumber, bool printPerson)
{
	PCRTest test(
		testId,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		DEFAULT_TIME_POINT,
		birthBumber,
		nullptr
	);

	TestWrapper key(&test);

	auto output = m_tests.find(&key);
	if (output != nullptr)
	{
		std::string result = output->getData()->result() ? "\nVysldedok: Pozitivny" : "\nVysledok: Negativny";

		if (printPerson)
		{
			return output->getData()->person()->toString() + result;
		}
		return result;
	}
	return "Record han't been found\n";
}

std::string Database::findTest(const unsigned int testId, bool printPerson)
{
	//TOTO TREBA ZMENIT NA INTERVALOVE PREHLADAVANIE

	PCRTest test(
		testId,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		DEFAULT_TIME_POINT,
		DEFAULT_STRING_VAL,
		nullptr
	);
	TestWrapper key(&test);

	auto output = m_tests.find(&key);
	if (output != nullptr)
	{
		if (printPerson)
		{
			return output->getData()->person()->toString() + output->getData()->toString();
		}
		return output->getData()->toString();
	}
	return "Record hasn't been found\n";
}

std::pair<bool, int> Database::removeTest(int testId)
{
	//TU NESKOR DOPLNIT PODMIENKU ABY SA MAZAL LEN TEN TEST KTORY SA MA MAZAT

	PCRTest test(
		testId,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		DEFAULT_TIME_POINT,
		DEFAULT_STRING_VAL,
		nullptr
	);
	TestWrapper key(&test);

	int count = 0;
	for (auto& testStructure : m_testStructuresList)
	{
		TestWrapper* removedTest;
		removedTest = m_tests.remove(&key);
	
		if (removedTest == nullptr)
		{
			return std::make_pair(false, 0);
		}

		if (testStructure == m_testStructuresList.back())
		{
			delete removedTest->getData();
			delete removedTest;
		}
		++count;
	}

	return std::make_pair(true, count);
}

std::pair<bool, int> Database::removePerson(std::string birthNumber)
{
	Person person(birthNumber, DEFAULT_STRING_VAL, DEFAULT_STRING_VAL, DEFAULT_DATE);
	PersonWrapper key(&person);

	PersonWrapper* removedPerson = m_people.remove(&key);
	if (removedPerson == nullptr)
	{
		return std::make_pair(false, 0);
	}

	int count = 1;
	AVLTree<TestWrapper*>& personsTests = removedPerson->getData()->tests();
	if (personsTests.size() > 0)
	{
		personsTests.processPostOrder([&count, this](TestWrapper* test) {
			for (auto& testStructure : m_testStructuresList)
			{
				testStructure->remove(test);
			}

			delete test->getData();
			delete test;

			++count;
		});
	}

	delete removedPerson->getData();
	delete removedPerson;
	return std::make_pair(true, count);
}

std::pair<std::string, int> Database::printAllData()
{
	int count = 0;
	std::ostringstream oss;

	m_people.processPreOrder([this, &oss, &count](PersonWrapper* person) {
		oss << person->getData()->toString() << "\n";
		++count;

		if (person->getData()->tests().size() > 0)
		{
			person->getData()->tests().processInOrder([&oss, &count](TestWrapper* test) {
				oss << test->getData()->toString() << "\n";
				++count;
			});
		}
	});

	return std::make_pair(oss.str(), count);
}

void Database::clear()
{
	if (m_people.size() > 0)
	{
		m_people.processPostOrder([](PersonWrapper* person) {
			delete person->getData();
			delete person;
		});
	}

	for (auto& testStructure : m_testStructuresList)
	{
		if (testStructure->size() > 0)
		{
			testStructure->processPostOrder([](TestWrapper* test) {
				delete test->getData();
				delete test;
			});
		}
	}
}

Database::~Database()
{
	clear();
}