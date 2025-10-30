#include "../../../Headers/Model/PCRDatabase/Database.h"


void Database::generateRandomPeople(int peopleCount)
{
	for (int i{}; i < peopleCount; ++i)
	{
		RandomDataGenerator::generatePeople(m_peopleList);
	}

	for (auto& person : m_peopleList)
	{
		m_people.insert(new PersonWrapper(person));
	}
}

void Database::generateRandomTests(int testCount)
{
	std::vector<PCRTest*> tests;

	for (int i{}; i < testCount; ++i)
	{
		RandomDataGenerator::generateTests(m_peopleList, tests);
	}

	for (auto& test : tests)
	{
		m_tests.insert(new TestWrapper(test));
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
	PCRTest test(
		testId,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		DEFAULT_TIME_POINT,
		birthBumber
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

		std::vector<TestWrapper*> output;
		PCRTest minKey(
			MIN_TEST_ID,
			DEFAULT_WORKPLACE,
			DEFAULT_DISTRICT,
			DEFAULT_REGION,
			DEFAULT_TEST_RES,
			DEFAULT_TEST_VAL,
			DEFAULT_STRING_VAL,
			DEFAULT_TIME_POINT,
			person->getData()->birthNumber()
		);
		PCRTest maxKey(
			MAX_TEST_ID,
			DEFAULT_WORKPLACE,
			DEFAULT_DISTRICT,
			DEFAULT_REGION,
			DEFAULT_TEST_RES,
			DEFAULT_TEST_VAL,
			DEFAULT_STRING_VAL,
			DEFAULT_TIME_POINT,
			person->getData()->birthNumber()
		);
		TestWrapper min(&minKey);
		TestWrapper max(&maxKey);

		m_tests.find(&min, &max, output);

		for (auto& test : output)
		{
			std::cout << test->getData()->toString();
		}
		std::cout << "================================\n";
	});

	m_tests.processInOrder([](TestWrapper* test) {
		std::cout << test->getData()->toString();
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