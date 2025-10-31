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

void Database::generateRandomTests(int testCount)
{
	if (m_people.size() > 0)
	{
		for (int i{}; i < testCount; ++i)
		{
			RandomDataGenerator::generateTests(m_peopleList, m_testStructuresList);
		}
	}
}

void Database::insert(Person* person)
{
	m_people.insert(new PersonWrapper(person));
}

void Database::insert(PCRTest* pcrTest)
{
	for (auto& testStructure : m_testStructuresList)
	{
		testStructure->insert(new TestWrapper(pcrTest));
	}
}

std::string Database::findTestResultByIdAndPatientId(const unsigned int testId, const std::string birthBumber)
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
		return output->getData()->toString();
	}
	return "";
}

void Database::printAllData()
{
	m_people.processPreOrder([this](PersonWrapper* person) {
		std::cout << person->getData()->toString() << "\n";
		
		if (person->getData()->tests().size() > 0)
		{
			person->getData()->tests().processInOrder([](TestWrapper* test) {
				std::cout << test->getData()->toString() << "\n";
			});
		}
	});
}

void Database::clear()
{
	m_people.processPostOrder([](PersonWrapper* person) {
		delete person->getData();
		delete person;
	});

	for (auto& testStructure : m_testStructuresList)
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