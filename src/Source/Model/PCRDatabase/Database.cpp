#include "../../../Headers/Model/PCRDatabase/Database.h"


Database::Database()
{
	m_testStructuresList.push_back(&m_tests);
	m_testStructuresList.push_back(&m_positiveTestsInDistrict);
	m_testStructuresList.push_back(&m_negativeTestsInDistrict);
	m_testStructuresList.push_back(&m_positiveTestsInRegion);
	m_testStructuresList.push_back(&m_negativeTestsInRegion);
	m_testStructuresList.push_back(&m_positiveTests);
	m_testStructuresList.push_back(&m_negativeTests);

	m_locationStructures.push_back(&m_districts);
	m_locationStructures.push_back(&m_regions);
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
			RandomDataGenerator::generateTests(m_peopleList, m_testStructuresList, m_locationStructures);
		}
		return true;
	}
	return false;
}

std::pair<std::string, int> Database::outputToString(std::vector<TestWrapper*>& output)
{
	if (output.size() <= 0)
	{
		return std::make_pair("No records found", 0);
	}

	int count = 0;
	std::ostringstream oss;
	for (auto& test : output)
	{
		oss << test->person()->getData()->toString() + "\n";
		oss << test->getData()->toString();
		++count;
	}

	return std::make_pair(oss.str(), count);
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

//(1)
bool Database::insert(TestWrapper* test)
{
	//NEZABUDNUT DOROBIT
	for (auto& testStructure : m_testStructuresList)
	{
		if (!testStructure->insert(test))
		{
			delete test;
			return false;
		}
		test->person()->tests().insert(new TestByDateWrapper(test->getData()));
	}
	return true;
}

//(2)
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

	TestWrapper* output = m_tests.find(&key);
	if (output == nullptr)
	{
		return "Record han't been found\n";
	}

	std::string result = output->getData()->result() ? "\nVysldedok: Pozitivny" : "\nVysledok: Negativny";
	if (printPerson)
	{
		return output->person()->getData()->toString() + result;
	}
	return result;
}

//(3)
std::pair<std::string, int> Database::findPatientTestOrderByDate(std::string birthNumber)
{
	std::ostringstream oss;

	Person person(birthNumber, DEFAULT_STRING_VAL, DEFAULT_STRING_VAL, DEFAULT_DATE);
	PersonWrapper key(&person);

	auto output = m_people.find(&key);
	if (output == nullptr)
	{
		return std::make_pair("", 0);
	}

	oss << output->getData()->toString() << "\n";
	int count = 0;

	if (output->tests().size() > 0)
	{
		output->tests().processInOrder([&oss, &count](TestByDateWrapper* test) {
			oss << test->getData()->toString() << "\n";
			++count;
		});
	}

	return std::make_pair(oss.str(), count);
}

//(4)
std::pair<std::string, int> Database::findPositiveTestsInDistrict(const unsigned int districtId, time_point<system_clock> from, time_point<system_clock> to)
{
	std::vector<TestWrapper*> output;
	PCRTest min(
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		districtId,
		DEFAULT_NUM_VAL,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		from,
		DEFAULT_STRING_VAL,
		nullptr
	);
	TestInDistrictWrapper minKey(&min);
	PCRTest max(
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		districtId,
		DEFAULT_NUM_VAL,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		to,
		DEFAULT_STRING_VAL,
		nullptr
	);
	TestInDistrictWrapper maxKey(&max);

	m_positiveTestsInDistrict.find(&minKey, &maxKey, output);	//DOPLNIT TEST ID DO COMPARATORA // ALEBOB POUZIT STROM DISTRICTOV A ODTIAL TAHAT TESTY

	return outputToString(output);
}

//(5)
std::pair<std::string, int> Database::findAllTestsInDistrict(const unsigned int districtId, time_point<system_clock> from, time_point<system_clock> to)
{
	auto positiveTestResult = findPositiveTestsInDistrict(districtId, from, to);

	std::vector<TestWrapper*> output;
	PCRTest min(
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		districtId,
		DEFAULT_NUM_VAL,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		from,
		DEFAULT_STRING_VAL,
		nullptr
	);
	TestInDistrictWrapper minKey(&min);
	PCRTest max(
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		districtId,
		DEFAULT_NUM_VAL,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		to,
		DEFAULT_STRING_VAL,
		nullptr
	);
	TestInDistrictWrapper maxKey(&max);

	m_negativeTestsInDistrict.find(&minKey, &maxKey, output);

	if (positiveTestResult.second == 0 && output.size() <= 0)
	{
		return positiveTestResult;
	}
	else if (positiveTestResult.second == 0)
	{
		positiveTestResult.first = "";
	}

	auto negativeTestResult = outputToString(output);

	return std::make_pair(positiveTestResult.first + "\n" + negativeTestResult.first, positiveTestResult.second + negativeTestResult.second);
}

//(6)
std::pair<std::string, int> Database::findPositiveTestsInRegion(const unsigned int regionId, time_point<system_clock> from, time_point<system_clock> to)
{
	std::vector<TestWrapper*> output;
	PCRTest min(
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		regionId,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		from,
		DEFAULT_STRING_VAL,
		nullptr
	);
	TestInRegionWrapper minKey(&min);
	PCRTest max(
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		regionId,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		to,
		DEFAULT_STRING_VAL,
		nullptr
	);
	TestInRegionWrapper maxKey(&max);

	m_positiveTestsInRegion.find(&minKey, &maxKey, output);	//DOPLNIT TEST ID DO COMPARATORA // ALEBOB POUZIT STROM REGIONOV A ODTIAL TAHAT TESTY

	return outputToString(output);
}

//(7)
std::pair<std::string, int> Database::findAllTestsInRegion(const unsigned int regionId, time_point<system_clock> from, time_point<system_clock> to)
{
	auto positiveTestResult = findPositiveTestsInRegion(regionId, from, to);

	std::vector<TestWrapper*> output;
	PCRTest min(
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		regionId,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		from,
		DEFAULT_STRING_VAL,
		nullptr
	);
	TestInRegionWrapper minKey(&min);
	PCRTest max(
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		regionId,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		to,
		DEFAULT_STRING_VAL,
		nullptr
	);
	TestInRegionWrapper maxKey(&max);

	m_negativeTestsInRegion.find(&minKey, &maxKey, output);

	if (positiveTestResult.second == 0 && output.size() <= 0)
	{
		return positiveTestResult;
	}
	else if (positiveTestResult.second == 0)
	{
		positiveTestResult.first = "";
	}

	auto negativeTestResult = outputToString(output);

	return std::make_pair(positiveTestResult.first + "\n" + negativeTestResult.first, positiveTestResult.second + negativeTestResult.second);
}

//(8)
std::pair<std::string, int> Database::findPositiveTests(time_point<system_clock> from, time_point<system_clock> to)
{
	std::vector<TestWrapper*> output;
	PCRTest min(
		MIN_ID,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		from,
		DEFAULT_STRING_VAL,
		nullptr
	);
	TestByDateWrapper minKey(&min);
	PCRTest max(
		MAX_ID,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		to,
		DEFAULT_STRING_VAL,
		nullptr
	);
	TestByDateWrapper maxKey(&max);

	m_positiveTests.find(&minKey, &maxKey, output);

	return outputToString(output);
}

//(9)
std::pair<std::string, int> Database::findAllTests(time_point<system_clock> from, time_point<system_clock> to)
{
	auto positiveTestResult = findPositiveTests(from, to);

	std::vector<TestWrapper*> output;
	PCRTest min(
		MIN_ID,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		from,
		DEFAULT_STRING_VAL,
		nullptr
	);
	TestByDateWrapper minKey(&min);
	PCRTest max(
		MAX_ID,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		to,
		DEFAULT_STRING_VAL,
		nullptr
	);
	TestByDateWrapper maxKey(&max);

	m_negativeTests.find(&minKey, &maxKey, output);

	if (positiveTestResult.second == 0 && output.size() <= 0)
	{
		return positiveTestResult;
	}
	else if (positiveTestResult.second == 0)
	{
		positiveTestResult.first = "";
	}

	auto negativeTestResult = outputToString(output);

	return std::make_pair(positiveTestResult.first + "\n" + negativeTestResult.first, positiveTestResult.second + negativeTestResult.second);
}

void Database::sickPeopleInDistrict(const unsigned int& districtId, time_point<system_clock>& from, time_point<system_clock>& to,
									LocationWrapper*& foundDistrict, std::vector<TestWrapper*>& output)
{
	LocationWrapper location(districtId);
	foundDistrict = m_districts.find(&location);

	if (foundDistrict == nullptr)
	{
		return;
	}

	PCRTest minTest(
		MIN_ID,
		DEFAULT_NUM_VAL,
		districtId,
		DEFAULT_NUM_VAL,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		from,
		DEFAULT_STRING_VAL,
		nullptr
	);
	TestByDateWrapper minKey(&minTest);
	PCRTest maxTest(
		MAX_ID,
		DEFAULT_NUM_VAL,
		districtId,
		DEFAULT_NUM_VAL,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		to,
		DEFAULT_STRING_VAL,
		nullptr
	);
	TestByDateWrapper maxKey(&maxTest);

	foundDistrict->positiveTests().find(&minKey, &maxKey, output);
}

//(10)
std::pair<std::string, int> Database::findSickPeopleInDistrict(const unsigned int districtId, time_point<system_clock> from, time_point<system_clock> to)
{
	LocationWrapper* foundDistrict;
	std::vector<TestWrapper*> output;
	sickPeopleInDistrict(districtId, from, to, foundDistrict, output);

	if (foundDistrict == nullptr)
	{
		return std::make_pair("District wasn't found", 0);
	}
	if (output.size() <= 0)
	{
		return std::make_pair("No tests in the district", 0);
	}

	return outputToString(output);
}

//(11)
std::pair<std::string, int> Database::findSickPeopleInDistrictOrderedByTestValue(const unsigned int districtId, time_point<system_clock> from, time_point<system_clock> to)
{
	LocationWrapper* foundDistrict;
	std::vector<TestWrapper*> output;
	sickPeopleInDistrict(districtId, from, to, foundDistrict, output);

	if (foundDistrict == nullptr)
	{
		return std::make_pair("District wasn't found", 0);
	}
	if (output.size() <= 0)
	{
		return std::make_pair("No tests in the district", 0);
	}

	std::sort(output.begin(), output.end(), [](TestWrapper* a, TestWrapper* b) {
		return a->getData()->testValue() < b->getData()->testValue();
	});

	return outputToString(output);
}

//(12)
std::pair<std::string, int> Database::findSickPeopleInRegion(const unsigned int regionId, time_point<system_clock> from, time_point<system_clock> to)
{
	LocationWrapper location(regionId);
	LocationWrapper* foundDistrict = m_districts.find(&location);

	if (foundDistrict == nullptr)
	{
		return std::make_pair("Region wasn't found", 0);
	}

	std::vector<TestWrapper*> output;
	PCRTest minTest(
		MIN_ID,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		regionId,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		from,
		DEFAULT_STRING_VAL,
		nullptr
	);
	TestByDateWrapper minKey(&minTest);
	PCRTest maxTest(
		MAX_ID,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		regionId,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		to,
		DEFAULT_STRING_VAL,
		nullptr
	);
	TestByDateWrapper maxKey(&maxTest);

	foundDistrict->positiveTests().find(&minKey, &maxKey, output);

	return outputToString(output);
}

//(13)
std::pair<std::string, int> Database::findSickPeople(time_point<system_clock> from, time_point<system_clock> to)
{
	std::vector<TestWrapper*> output;
	PCRTest minTest(
		MIN_ID,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		from,
		DEFAULT_STRING_VAL,
		nullptr
	);
	TestByDateWrapper minKey(&minTest);
	PCRTest maxTest(
		MAX_ID,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		to,
		DEFAULT_STRING_VAL,
		nullptr
	);
	TestByDateWrapper maxKey(&maxTest);

	m_positiveTests.find(&minKey, &maxKey, output);

	return outputToString(output);
}

//(14)
std::pair<std::string, int> Database::findMostSickPersonInDistrict()
{
	//TOTO CELE ZLE - DOPLNIT PARAMETRE PRE INTERVALOVE VYHLADAVANIE

	if (m_districts.size() == 0)
	{
		return std::make_pair("No district in database", 0);
	}

	int count = 0;
	std::ostringstream oss;
	m_districts.processInOrder([&oss, &count](LocationWrapper* district) {
		auto test = district->positiveTests().findMaxKey();	//TUTO BUDE PRAVDEPODOBNE DALSIA STRUKTURA
		oss << "District " << district->locationId() << ":\n";
		oss << test->person()->getData()->toString() << "\n";
		++count;
	});

	std::make_pair(oss.str(), count);
}

//(18)
std::string Database::findTest(const unsigned int testId, bool printPerson)
{
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
			return output->person()->getData()->toString() + "\n" + output->getData()->toString();
		}
		return output->getData()->toString();
	}
	return "Record hasn't been found\n";
}

//(19)
bool Database::insert(PersonWrapper* personWrapper)
{
	//NEZABUDNUT DOROBIT
	if (m_people.insert(personWrapper))
	{
		return true;
	}

	delete personWrapper;
	return false;
}

//(20)
int Database::removeTest(int testId)
{
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

		if (removedTest != nullptr)
		{
			if (testStructure == m_testStructuresList.back())
			{
				TestByDateWrapper testByDate(&test);
				removedTest->person()->tests().remove((&testByDate));

				delete removedTest->getData();
				delete removedTest;
			}
			++count;
		}
	}

	return count;
}

//(21)
std::pair<bool, int> Database::removePerson(std::string birthNumber)
{
	//Person person(birthNumber, DEFAULT_STRING_VAL, DEFAULT_STRING_VAL, DEFAULT_DATE);
	//PersonWrapper key(&person);

	//PersonWrapper* removedPerson = m_people.remove(&key);
	//if (removedPerson == nullptr)
	//{
	//	return std::make_pair(false, 0);
	//}

	//int count = 1;
	//AVLTree<TestByDateWrapper*>& personsTests = removedPerson->tests();
	//if (personsTests.size() > 0)
	//{
	//	personsTests.processPostOrder([&count, this](TestByDateWrapper* test) {
	//		for (auto& testStructure : m_testStructuresList)
	//		{
	//			TestWrapper key(test->getData());
	//			testStructure->remove(&key);
	//		}

	//		delete test->getData();
	//		delete test;

	//		++count;
	//	});
	//}

	//delete removedPerson->getData();
	//delete removedPerson;
	//return std::make_pair(true, count);

	return std::make_pair(false, 0);
}

std::pair<std::string, int> Database::printAllData()
{
	int count = 0;
	std::ostringstream oss;

	m_people.processPreOrder([this, &oss, &count](PersonWrapper* person) {
		oss << person->getData()->toString() << "\n";
		++count;

		if (person->tests().size() > 0)
		{
			person->tests().processInOrder([&oss, &count](TestByDateWrapper* test) {
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
			if (person->tests().size() > 0)
			{
				person->tests().processPostOrder([](TestWrapper* test) {
					delete test;
				});
			}

			delete person->getData();
			delete person;
		});
	}

	for (auto& location : m_locationStructures)
	{
		if (location->size() > 0)
		{
			location->processPostOrder([](LocationWrapper* location) {
				if (location->positiveTests().size() > 0)
				{
					location->positiveTests().processPostOrder([](TestWrapper* test) {
						delete test;
					});
				}
				if (location->negativeTests().size() > 0)
				{
					location->negativeTests().processPostOrder([](TestWrapper* test) {
						delete test;
					});
				}

				delete location;
			});
		}
	}

	for (int i = 1; i < m_testStructuresList.size(); ++i)
	{
		if (m_testStructuresList.at(i)->size() > 0)
		{
			m_testStructuresList.at(i)->processPostOrder([](TestWrapper* test) {
				delete test;
			});
		}
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