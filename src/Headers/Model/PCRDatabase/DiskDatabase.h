#pragma once
#include "Database.h"
#include "ModelWrappers/PersonHashWrapper.h"
#include "ModelWrappers/TestHashWrapper.h"
#include "Generator/RandomDataGenerator.h"
#include "../HashFile/HashFile.h"

class DiskDatabase : public Database
{
private:
	static constexpr const char* PEOPLE_FILE_PATH = "../../../data/HashFile/People/";
	static constexpr const char* TESTS_FILE_PATH = "../../../data/HashFile/Tests/";
	static constexpr const int PEOPLE_PRIMARY_CLUSTERS_SIZE = 800;
	static constexpr const int PEOPLE_OVERFLOW_CLUSTERS_SIZE = 300;
	static constexpr const int TESTS_PRIMARY_CLUSTERS_SIZE = 800;
	static constexpr const int TESTS_OVERFLOW_CLUSTERS_SIZE = 300;

	std::random_device m_rd;
	std::mt19937 m_gen{ m_rd() };

	std::vector<PersonHashWrapper*> m_peopleList;

	HashFile<PersonHashWrapper> m_people;
	HashFile<TestHashWrapper> m_tests;

public:
	DiskDatabase();

	void generateRandomPeople(int peopleCount) override;

	bool generateRandomTests(int testCount) override;

	void insert(PersonHashWrapper* person);

	void insert(TestHashWrapper* test);

	std::pair<std::string, int> printAllData() override;

	void clear() override;

	~DiskDatabase() = default;
};