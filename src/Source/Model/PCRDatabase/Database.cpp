#include "../../../Headers/Model/PCRDatabase/Database.h"


Database::Database()
{
	m_testStructuresList = std::make_pair(&m_positiveTests, &m_negativeTests);
	m_locationStructures.push_back(&m_regions);
	m_locationStructures.push_back(&m_districts);
	m_locationStructures.push_back(&m_workplaces);
}

std::pair<std::string, int> Database::outputToString(std::vector<TestByDateWrapper*>& output)
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
			RandomDataGenerator::generateTests(m_peopleList, m_testStructuresList, m_tests, m_locationStructures);
		}
		return true;
	}
	return false;
}

PersonWrapper* Database::findPerson(std::string birthNumber)
{
	Person person(
		birthNumber,
		DEFAULT_STRING_VAL,
		DEFAULT_STRING_VAL,
		DEFAULT_DATE
	);
	PersonWrapper key(&person);

	return m_people.find(&key);
}

//(1)
bool Database::insert(TestByDateWrapper* test)
{
	bool inserted;

	if (test->getData()->result())
	{
		inserted = m_positiveTests.insert(test);
	}
	else
	{
		inserted = m_negativeTests.insert(test);
	}

	LocationWrapper* region =  new LocationWrapper(test->getData()->regionId());
	LocationWrapper* district = new LocationWrapper(test->getData()->districtId());
	LocationWrapper* workplace = new LocationWrapper(test->getData()->workplaceId());

	if (inserted)
	{
		auto testData = test->getData();
		m_tests.insert(new TestWrapper(test->getData(), test->person()));
		findPerson(test->getData()->birthNumber())->tests().insert(test);

		if (!m_regions.insert(region))
		{
			LocationWrapper* foundRegion = m_regions.find(region);
			delete region;
			region = foundRegion;
		}
		if (!m_districts.insert(district))
		{
			LocationWrapper* foundDistrict = m_districts.find(district);
			delete district;
			district = foundDistrict;
		}
		if (!m_workplaces.insert(workplace))
		{
			LocationWrapper* foundWorkplace = m_workplaces.find(workplace);
			delete workplace;
			workplace = foundWorkplace;
		}

		if (test->getData()->result())
		{
			region->positiveTests().insert(test);
			district->positiveTests().insert(test);
			workplace->positiveTests().insert(test);
		}
		else
		{
			region->negativeTests().insert(test);
			district->negativeTests().insert(test);
			workplace->negativeTests().insert(test);
		}
	}
	else
	{
		delete test->getData();
		delete test;
		delete region;
		delete district;
		delete workplace;
	}

	return inserted;
}

//(2)
std::string Database::findTestResultByIdAndPatientId(const unsigned int testId, const std::string birthBumber)
{
	return findTest(testId);
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
	LocationWrapper district(districtId);
	LocationWrapper* foundDistrict = m_districts.find(&district);

	if (foundDistrict == nullptr)
	{
		return std::make_pair("District wasn't found", 0);
	}

	std::vector<TestByDateWrapper*> output;
	PCRTest min(
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		districtId,
		DEFAULT_NUM_VAL,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		from,
		DEFAULT_STRING_VAL
	);
	TestByDateWrapper minKey(&min);
	PCRTest max(
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		districtId,
		DEFAULT_NUM_VAL,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		to,
		DEFAULT_STRING_VAL
	);
	TestByDateWrapper maxKey(&max);

	foundDistrict->positiveTests().find(&minKey, &maxKey, output);

	return outputToString(output);
}

//(5)
std::pair<std::string, int> Database::findAllTestsInDistrict(const unsigned int districtId, time_point<system_clock> from, time_point<system_clock> to)
{
	LocationWrapper district(districtId);
	LocationWrapper* foundDistrict = m_districts.find(&district);

	if (foundDistrict == nullptr)
	{
		return std::make_pair("District wasn't found", 0);
	}

	PCRTest min(
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		districtId,
		DEFAULT_NUM_VAL,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		from,
		DEFAULT_STRING_VAL
	);
	TestByDateWrapper minKey(&min);
	PCRTest max(
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		districtId,
		DEFAULT_NUM_VAL,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		to,
		DEFAULT_STRING_VAL
	);
	TestByDateWrapper maxKey(&max);

	std::vector<TestByDateWrapper*> positiveOutput;
	std::vector<TestByDateWrapper*> negativeOutput;

	foundDistrict->positiveTests().find(&minKey, &maxKey, positiveOutput);
	foundDistrict->negativeTests().find(&minKey, &maxKey, negativeOutput);

	auto positiveResult = outputToString(positiveOutput);
	auto negativeResult = outputToString(negativeOutput);

	return std::make_pair(positiveResult.first + "\n" + negativeResult.first, positiveResult.second + negativeResult.second);
}

//(6)
std::pair<std::string, int> Database::findPositiveTestsInRegion(const unsigned int regionId, time_point<system_clock> from, time_point<system_clock> to)
{
	LocationWrapper region(regionId);
	LocationWrapper* foundRegion = m_regions.find(&region);

	if (foundRegion == nullptr)
	{
		return std::make_pair("Region wasn't found", 0);
	}

	std::vector<TestByDateWrapper*> output;
	PCRTest min(
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		regionId,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		from,
		DEFAULT_STRING_VAL
	);
	TestByDateWrapper minKey(&min);
	PCRTest max(
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		regionId,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		to,
		DEFAULT_STRING_VAL
	);
	TestByDateWrapper maxKey(&max);

	foundRegion->positiveTests().find(&minKey, &maxKey, output);

	return outputToString(output);
}

//(7)
std::pair<std::string, int> Database::findAllTestsInRegion(const unsigned int regionId, time_point<system_clock> from, time_point<system_clock> to)
{
	LocationWrapper region(regionId);
	LocationWrapper* foundRegion = m_regions.find(&region);

	if (foundRegion == nullptr)
	{
		return std::make_pair("Region wasn't found", 0);
	}

	PCRTest min(
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		regionId,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		from,
		DEFAULT_STRING_VAL
	);
	TestByDateWrapper minKey(&min);
	PCRTest max(
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		regionId,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		to,
		DEFAULT_STRING_VAL
	);
	TestByDateWrapper maxKey(&max);

	std::vector<TestByDateWrapper*> positiveOutput;
	std::vector<TestByDateWrapper*> negativeOutput;

	foundRegion->positiveTests().find(&minKey, &maxKey, positiveOutput);
	foundRegion->negativeTests().find(&minKey, &maxKey, negativeOutput);

	auto positiveResult = outputToString(positiveOutput);
	auto negativeResult = outputToString(negativeOutput);

	return std::make_pair(positiveResult.first + "\n" + negativeResult.first, positiveResult.second + negativeResult.second);
}

//(8)
std::pair<std::string, int> Database::findPositiveTests(time_point<system_clock> from, time_point<system_clock> to)
{
	std::vector<TestByDateWrapper*> output;
	PCRTest min(
		MIN_ID,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		from,
		DEFAULT_STRING_VAL
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
		DEFAULT_STRING_VAL
	);
	TestByDateWrapper maxKey(&max);

	m_positiveTests.find(&minKey, &maxKey, output);

	return outputToString(output);
}

//(9)
std::pair<std::string, int> Database::findAllTests(time_point<system_clock> from, time_point<system_clock> to)
{
	PCRTest min(
		MIN_ID,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		from,
		DEFAULT_STRING_VAL
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
		DEFAULT_STRING_VAL
	);
	TestByDateWrapper maxKey(&max);

	std::vector<TestByDateWrapper*> positiveOutput;
	std::vector<TestByDateWrapper*> negativeOutput;

	m_positiveTests.find(&minKey, &maxKey, positiveOutput);
	m_negativeTests.find(&minKey, &maxKey, negativeOutput);

	auto positiveResult = outputToString(positiveOutput);
	auto negativeResult = outputToString(negativeOutput);

	return std::make_pair(positiveResult.first + "\n" + negativeResult.first, positiveResult.second + negativeResult.second);
}

void Database::sickPeopleInDistrict(const unsigned int& districtId, time_point<system_clock>& from, time_point<system_clock>& to,
									LocationWrapper*& foundDistrict, std::vector<TestByDateWrapper*>& output)
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
		DEFAULT_STRING_VAL
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
		DEFAULT_STRING_VAL
	);
	TestByDateWrapper maxKey(&maxTest);

	foundDistrict->positiveTests().find(&minKey, &maxKey, output);
}

//(10)
std::pair<std::string, int> Database::findSickPeopleInDistrict(const unsigned int districtId, time_point<system_clock> from, time_point<system_clock> to)
{
	LocationWrapper* foundDistrict;
	std::vector<TestByDateWrapper*> output;
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
	std::vector<TestByDateWrapper*> output;
	sickPeopleInDistrict(districtId, from, to, foundDistrict, output);

	if (foundDistrict == nullptr)
	{
		return std::make_pair("District wasn't found", 0);
	}
	if (output.size() <= 0)
	{
		return std::make_pair("No tests in the district", 0);
	}

	std::sort(output.begin(), output.end(), [](TestByDateWrapper* a, TestByDateWrapper* b) {
		return a->getData()->testValue() > b->getData()->testValue();
	});

	return outputToString(output);
}

//(12)
std::pair<std::string, int> Database::findSickPeopleInRegion(const unsigned int regionId, time_point<system_clock> from, time_point<system_clock> to)
{
	LocationWrapper region(regionId);
	LocationWrapper* foundRegion = m_regions.find(&region);

	if (foundRegion == nullptr)
	{
		return std::make_pair("Region wasn't found", 0);
	}

	std::vector<TestByDateWrapper*> output;
	PCRTest minTest(
		MIN_ID,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		regionId,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		from,
		DEFAULT_STRING_VAL
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
		DEFAULT_STRING_VAL
	);
	TestByDateWrapper maxKey(&maxTest);

	foundRegion->positiveTests().find(&minKey, &maxKey, output);

	return outputToString(output);
}

//(13)
std::pair<std::string, int> Database::findSickPeople(time_point<system_clock> from, time_point<system_clock> to)
{
	std::vector<TestByDateWrapper*> output;
	PCRTest minTest(
		MIN_ID,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		from,
		DEFAULT_STRING_VAL
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
		DEFAULT_STRING_VAL
	);
	TestByDateWrapper maxKey(&maxTest);

	m_positiveTests.find(&minKey, &maxKey, output);

	int count = 0;
	std::ostringstream oss;
	for (auto& test : output)
	{
		oss << test->person()->getData()->toString() << "\n";
		++count;
	}

	return std::make_pair(oss.str(), count);
}

//(14)
std::pair<std::string, int> Database::findMostSickPersonInDistrict(time_point<system_clock> from, time_point<system_clock> to)
{
	if (m_districts.size() == 0)
	{
		return std::make_pair("There are no districts in database yet", 0);
	}

	int count = 0;
	std::ostringstream oss;
	m_districts.processInOrder([this, &oss, &count, &from, &to](LocationWrapper* district) {
		std::vector<TestByDateWrapper*> output;
		PCRTest minTest(
			MIN_ID,
			DEFAULT_NUM_VAL,
			DEFAULT_NUM_VAL,
			DEFAULT_NUM_VAL,
			DEFAULT_BOOL_VAL,
			DEFAULT_NUM_VAL,
			DEFAULT_STRING_VAL,
			from,
			DEFAULT_STRING_VAL
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
			DEFAULT_STRING_VAL
		);
		TestByDateWrapper maxKey(&maxTest);

		district->positiveTests().find(&minKey, &maxKey, output);

		++count;
		oss << "\nDistrict (" << district->locationId() << "): ";
		if (output.size() > 0)
		{
			TestByDateWrapper* mostSickPerson = nullptr;
			for (auto test : output)
			{
				if (mostSickPerson == nullptr || mostSickPerson->getData()->testValue() < test->getData()->testValue())
				{
					mostSickPerson = test;
				}
			}
			oss << mostSickPerson->person()->getData()->toString() << "\n";
		}
		else
		{
			oss << "District has no sick people to this date\n";
		}
	});

	return std::make_pair(oss.str(), count);
}

//(15)
std::pair<std::string, int> Database::findDistrictsOrderedBySickPeopleCount(time_point<system_clock> from, time_point<system_clock> to)
{
	if (m_districts.size() == 0)
	{
		return std::make_pair("There are no districts in database yet", 0);
	}

	std::vector<std::pair<int, int>> sickPeopleInDistrict;

	m_districts.processInOrder([this, &sickPeopleInDistrict, &from, &to](LocationWrapper* district) {
		std::vector<TestByDateWrapper*> output;
		PCRTest minTest(
			MIN_ID,
			DEFAULT_NUM_VAL,
			DEFAULT_NUM_VAL,
			DEFAULT_NUM_VAL,
			DEFAULT_BOOL_VAL,
			DEFAULT_NUM_VAL,
			DEFAULT_STRING_VAL,
			from,
			DEFAULT_STRING_VAL
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
			DEFAULT_STRING_VAL
		);
		TestByDateWrapper maxKey(&maxTest);

		district->positiveTests().find(&minKey, &maxKey, output);
		sickPeopleInDistrict.push_back(std::make_pair(district->locationId(), output.size()));
	});

	std::sort(sickPeopleInDistrict.begin(), sickPeopleInDistrict.end(), [](auto& a, auto& b) {
		return a.second > b.second;
	});

	int count = 0;
	std::ostringstream oss;
	for (auto& district : sickPeopleInDistrict)
	{
		oss << "\nDistrict (" << district.first << "): ";
		oss << district.second << "sick people\n";
		++count;
	}

	return std::make_pair(oss.str(), count);
}

//(16)
std::pair<std::string, int> Database::findRegionsOrderedBySickPeopleCount(time_point<system_clock> from, time_point<system_clock> to)
{
	if (m_regions.size() == 0)
	{
		return std::make_pair("There are no regions in database yet", 0);
	}

	std::vector<std::pair<int, int>> sickPeopleInRegion;

	m_regions.processInOrder([this, &sickPeopleInRegion, &from, &to](LocationWrapper* region) {
		std::vector<TestByDateWrapper*> output;
		PCRTest minTest(
			MIN_ID,
			DEFAULT_NUM_VAL,
			DEFAULT_NUM_VAL,
			DEFAULT_NUM_VAL,
			DEFAULT_BOOL_VAL,
			DEFAULT_NUM_VAL,
			DEFAULT_STRING_VAL,
			from,
			DEFAULT_STRING_VAL
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
			DEFAULT_STRING_VAL
		);
		TestByDateWrapper maxKey(&maxTest);

		region->positiveTests().find(&minKey, &maxKey, output);
		sickPeopleInRegion.push_back(std::make_pair(region->locationId(), output.size()));
		});

	std::sort(sickPeopleInRegion.begin(), sickPeopleInRegion.end(), [](auto& a, auto& b) {
		return a.second > b.second;
	});

	int count = 0;
	std::ostringstream oss;
	for (auto& region : sickPeopleInRegion)
	{
		oss << "\nDistrict (" << region.first << "): ";
		oss << region.second << " sick people\n";
		++count;
	}

	return std::make_pair(oss.str(), count);
}

//(17)
std::pair<std::string, int> Database::findAllTestsAtWorkplace(int workplaceId, time_point<system_clock> from, time_point<system_clock> to)
{
	LocationWrapper workplace(workplaceId);
	LocationWrapper* foundWorkplace = m_workplaces.find(&workplace);

	if (foundWorkplace == nullptr)
	{
		return std::make_pair("Workplace wasn't found", 0);
	}

	PCRTest minTest(
		MIN_ID,
		workplaceId,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		from,
		DEFAULT_STRING_VAL
	);
	TestByDateWrapper minKey(&minTest);
	PCRTest maxTest(
		MAX_ID,
		workplaceId,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		to,
		DEFAULT_STRING_VAL
	);
	TestByDateWrapper maxKey(&maxTest);

	std::vector<TestByDateWrapper*> positiveOutput;
	std::vector<TestByDateWrapper*> negativeOutput;

	foundWorkplace->positiveTests().find(&minKey, &maxKey, positiveOutput);
	foundWorkplace->negativeTests().find(&minKey, &maxKey, negativeOutput);

	auto positiveResult = outputToString(positiveOutput);
	auto negativeResult = outputToString(negativeOutput);

	return std::make_pair(positiveResult.first + "\n" + negativeResult.first, positiveResult.second + negativeResult.second);
}

TestWrapper* Database::findTestWrapper(int testId)
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
		DEFAULT_STRING_VAL
	);
	TestWrapper key(&test);

	return m_tests.find(&key);
}

//(18)
std::string Database::findTest(const unsigned int testId)
{
	auto output = findTestWrapper(testId);
	if (output != nullptr)
	{
		return output->person()->getData()->toString() + "\n" + output->getData()->toString();
	}
	return "Record hasn't been found\n";
}

//(19)
bool Database::insert(PersonWrapper* personWrapper)
{
	if (m_people.insert(personWrapper))
	{
		m_peopleList.push_back(personWrapper);
		return true;
	}

	delete personWrapper->getData();
	delete personWrapper;
	return false;
}

//(20)
int Database::removeTest(int testId)
{
	PCRTest testDummy(
		testId,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_BOOL_VAL,
		DEFAULT_NUM_VAL,
		DEFAULT_STRING_VAL,
		DEFAULT_TIME_POINT,
		DEFAULT_STRING_VAL
	);
	TestWrapper key(&testDummy);

	int count = 0;
	auto testWrapper = m_tests.remove(&key);

	if (testWrapper != nullptr)
	{
		PCRTest testByDateDummy(
			testId,
			DEFAULT_NUM_VAL,
			DEFAULT_NUM_VAL,
			DEFAULT_NUM_VAL,
			DEFAULT_BOOL_VAL,
			DEFAULT_NUM_VAL,
			DEFAULT_STRING_VAL,
			testWrapper->getData()->testDate(),
			DEFAULT_STRING_VAL
		);
		TestByDateWrapper keyWithDate(&testByDateDummy);

		testWrapper->person()->tests().remove(&keyWithDate);

		LocationWrapper region(testWrapper->getData()->regionId());
		LocationWrapper district(testWrapper->getData()->districtId());
		LocationWrapper workplace(testWrapper->getData()->workplaceId());
		if (testWrapper->getData()->result())
		{
			m_regions.find(&region)->positiveTests().remove(&keyWithDate);
			m_districts.find(&district)->positiveTests().remove(&keyWithDate);
			m_workplaces.find(&workplace)->positiveTests().remove(&keyWithDate);
			delete m_positiveTests.remove(&keyWithDate);
		}
		else
		{
			m_regions.find(&region)->negativeTests().remove(&keyWithDate);
			m_districts.find(&district)->negativeTests().remove(&keyWithDate);
			m_workplaces.find(&workplace)->negativeTests().remove(&keyWithDate);
			delete m_negativeTests.remove(&keyWithDate);
		}
		delete testWrapper->getData();
		delete testWrapper;
		++count;
	}

	return count;
}

//(21)
int Database::removePerson(std::string birthNumber)
{
	Person person(
		birthNumber,
		DEFAULT_STRING_VAL,
		DEFAULT_STRING_VAL,
		DEFAULT_DATE
	);
	PersonWrapper key(&person);

	PersonWrapper* foundPerson = m_people.remove(&key);
	
	int count = 0;
	if (foundPerson != nullptr)
	{
		if (foundPerson->tests().size() > 0)
		{
			foundPerson->tests().processPostOrder([this, &count](TestByDateWrapper* test) {
				count += removeTest(test->getData()->testId());
			});
		}
		delete foundPerson->getData();
		delete foundPerson;
		++count;
	}

	return count;
}

std::pair<std::string, int> Database::printAllData()
{
	if (m_people.size() == 0)
	{
		return std::make_pair("Database is empty", 0);
	}

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

std::pair<std::string, int> Database::saveToFile()
{
	namespace fs = std::filesystem;

	fs::path peopleFilePath = PEOPLE_FILE_PATH;
	fs::path testFilePath = TESTS_FILE_PATH;

	if (!fs::exists(peopleFilePath))
	{
		std::ofstream file(peopleFilePath);
	}
	if (!fs::exists(testFilePath))
	{
		std::ofstream file(testFilePath);
	}

	int count = 0;
	//People
	if (m_people.size() > 0)
	{
		std::ofstream peopleFile(peopleFilePath);
		if (!peopleFile.is_open())
		{
			return std::make_pair("Couldn't open the people file", 0);
		}

		m_people.processLevelOrder([&peopleFile, &count](PersonWrapper* person) {
			peopleFile << person->writeLine();
			++count;
		});

		peopleFile.close();
	}

	//Tests
	if (m_tests.size() > 0)
	{
		std::ofstream testFile(testFilePath);
		if (!testFile.is_open())
		{
			return std::make_pair("Couldn't open the tests file", 0);
		}

		m_tests.processLevelOrder([&testFile, &count](TestWrapper* test) {
			testFile << test->writeLine();
			++count;
		});

		testFile.close();
	}

	return std::make_pair("All the data were saved to file located inside applications data folder", count);
}

std::pair<std::string, int> Database::loadFromFile()
{
	namespace fs = std::filesystem;

	fs::path peopleFilePath = PEOPLE_FILE_PATH;
	fs::path testFilePath = TESTS_FILE_PATH;

	int count = 0;
	std::string line;

	//People
	std::ifstream peopleFile(peopleFilePath);
	if (!peopleFile.is_open())
	{
		return std::make_pair("Couldn't open the people file", 0);
	}

	while (std::getline(peopleFile, line))
	{
		if (line.find(';') == std::string::npos)
		{
			continue;
		}

		m_people.insert(static_cast<PersonWrapper*>(PersonWrapper::loadLine(line)));
		++count;
	}
	peopleFile.close();

	//Tests
	std::ifstream testsFile(testFilePath);
	if (!testsFile.is_open())
	{
		return std::make_pair("Couldn't open the tests file", 0);
	}

	while (std::getline(testsFile, line))
	{
		if (line.find(';') == std::string::npos)
		{
			continue;
		}

		TestWrapper* test = static_cast<TestWrapper*>(TestWrapper::loadLine(line));
		TestByDateWrapper* testByDateWrapper = new TestByDateWrapper(test->getData());
		insert(testByDateWrapper);
		delete test;

		++count;
	}
	testsFile.close();

	return std::make_pair("Data were loaded successfuly", count);
}

void Database::clear()
{
	if (m_people.size() > 0)
	{
		m_people.processPostOrder([](PersonWrapper* person) {
			delete person->getData();
			delete person;
		});
		m_people.clear();
		m_peopleList.clear();
	}
	for (auto& location : m_locationStructures)
	{
		if (location->size() > 0)
		{
			location->processPostOrder([](LocationWrapper* location) {
				delete location;
			});
		}
		location->clear();
	}
	if (m_positiveTests.size() > 0)
	{
		m_positiveTests.processPostOrder([](TestByDateWrapper* test) {
			delete test;
		});
		m_positiveTests.clear();
	}
	if (m_negativeTests.size() > 0)
	{
		m_negativeTests.processPostOrder([](TestByDateWrapper* test) {
			delete test;
		});
		m_negativeTests.clear();
	}
	if (m_tests.size() > 0)
	{
		m_tests.processPostOrder([](TestWrapper* test) {
			delete test->getData();
			delete test;
		});
		m_tests.clear();
	}
}

Database::~Database()
{
	clear();
}