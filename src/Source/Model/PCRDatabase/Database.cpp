#include "../../../Headers/Model/PCRDatabase/Database.h"


void Database::generateRandomData(int dataCount)
{
	std::vector<Person*> people;
	std::vector<PCRTest*> tests;

	for (int i{}; i < dataCount; ++i)
	{
		RandomDataGenerator::generatePeople(people);
	}

	for (int i{}; i < dataCount; ++i)
	{
		RandomDataGenerator::generateTests(people, tests);
	}
	
	for (auto& test : tests)
	{
		m_tests.insert(new TestWrapper(test));
	}

	for (auto& person : people)
	{
		delete person;
	}
}

void Database::insert()
{

}

void Database::clear()
{

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

	return m_tests.find(&key)->getData()->toString();
}

void Database::printAllData()
{
	m_tests.processInOrder([](TestWrapper* test) {
		std::cout << test->getData()->toString();
	});

	//m_people.processPreOrder([this](Person* person) {
	//	std::cout << person->toString();

	//	std::vector<TestWrapper*> output;
	//	TestWrapper testMin(new PCRTest(-1, 0, 0, 0, 0, 0, "", std::chrono::system_clock::now(), "0"));
	//	TestWrapper testMax(new PCRTest(999999999999999, 9, 9, 9, 9, 9, "", std::chrono::system_clock::now(), "zzzzzzzzzzzzzzzzzzzzzz"));

	//	m_tests.find(&testMin, &testMax, output);

	//	for (auto& test : output)
	//	{
	//		std::cout << test->getData()->toString();
	//	}

	//	delete testMin.getData();
	//	delete testMax.getData();
	//});
}

Database::~Database()
{
	m_tests.processPostOrder([](TestWrapper* test) {
		delete test->getData();
		delete test;
	});
}