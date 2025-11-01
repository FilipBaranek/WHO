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

std::string Database::printAllData()
{
	std::ostringstream oss;

	m_people.processPreOrder([this, &oss](PersonWrapper* person) {
		oss << person->getData()->toString() << "\n";
		
		if (person->getData()->tests().size() > 0)
		{
			person->getData()->tests().processInOrder([&oss](TestWrapper* test) {
				oss << test->getData()->toString() << "\n";
			});
		}
	});

	return oss.str();
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

	if (m_tests.size() > 0)
	{
		m_tests.processPostOrder([](TestWrapper* test) {
			delete test->getData();
			delete test;
		});
	}
}

Database::~Database()
{
	clear();
}