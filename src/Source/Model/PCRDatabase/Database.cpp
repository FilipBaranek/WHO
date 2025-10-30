#include "../../../Headers/Model/PCRDatabase/Database.h"


void Database::generateRandomPeople(int peopleCount)
{
	std::vector<Person*> people;

	for (int i{}; i < peopleCount; ++i)
	{
		RandomDataGenerator::generatePeople(people);
	}

	for (auto& person : people)
	{
		m_people.insert(new PersonWrapper(person));
	}
}

void Database::generateRandomTests(int testCount)
{
	if (m_people.size() > 0)
	{
		std::vector<PCRTest*> tests;

		for (int i{}; i < testCount; ++i)
		{
			RandomDataGenerator::generateTests(m_people, tests);
		}

		for (auto& test : tests)
		{
			m_tests.insert(new TestWrapper(test));

			Person person(test->birthNumber(), DEFAULT_STRING_VAL, DEFAULT_STRING_VAL, DEFAULT_DATE);
			PersonWrapper key(&person);

			m_people.find(&key)->getData()->tests().insert(new TestWrapper(test));
		}
	}
}

void Database::insert(Person* person)
{
	m_people.insert(new PersonWrapper(person));
}

void Database::insert(PCRTest* pcrTest)
{
	m_tests.insert(new TestWrapper(pcrTest));
}

std::string Database::findTestResultByIdAndPatientId(const unsigned int testId, const std::string birthBumber)
{
	//DOROBIT ABY VRACALO OUTPUT PODLA ZADANIA
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

	m_tests.processPostOrder([](TestWrapper* test) {
		delete test->getData();
		delete test;
	});
}

Database::~Database()
{
	clear();
}