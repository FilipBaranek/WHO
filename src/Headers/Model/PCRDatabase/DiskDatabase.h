#pragma once
#include "Database.h"
#include "Models/Person.h"
#include "Models/PCRTest.h"
#include "Generator/RandomDataGenerator.h"
#include "../HashFile/HashFile.h"

class DiskDatabase : public Database
{
private:
	static constexpr const char* PEOPLE_FILE_PATH = "../../../data/HashFile/People/";
	static constexpr const char* TESTS_FILE_PATH = "../../../data/HashFile/People/";
	static constexpr const int PEOPLE_PRIMARY_CLUSTERS_SIZE = 800;
	static constexpr const int PEOPLE_OVERFLOW_CLUSTERS_SIZE = 300;
	static constexpr const int TESTS_PRIMARY_CLUSTERS_SIZE = 800;
	static constexpr const int TESTS_OVERFLOW_CLUSTERS_SIZE = 300;

	std::random_device m_rd;
	std::mt19937 m_gen{ m_rd() };

	//HashFile<Person> m_people;
	//HashFile<PCRTest> m_tests;

public:
	DiskDatabase();

	void generateRandomPeople(int peopleCount) override;

	bool generateRandomTests(int testCount) override;

	void insert(Person* person);

	std::pair<std::string, int> printAllData() override;

	void clear() override;

	~DiskDatabase() = default;
};