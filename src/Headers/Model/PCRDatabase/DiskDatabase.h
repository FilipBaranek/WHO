#pragma once
#include "Database.h"
#include "../HeapFile/HeapFile.h"
#include "Models/Person.h"
#include "Models/PCRTest.h"
#include "Generator/RandomDataGenerator.h"

class DiskDatabase : public Database
{
private:
	static constexpr const int PEOPLE_CLUSTERS_SIZE = 400;
	static constexpr const int TESTS_CLUSTERS_SIZE = 400;
	static constexpr const char* FILE_PATH = "../../../data/";

	std::random_device m_rd;
	std::mt19937 m_gen{ m_rd() };

	HeapFile<Person> m_people;
	//HeapFile<PCRTest> m_tests;

public:
	DiskDatabase();

	void generateRandomPeople(int peopleCount) override;

	bool generateRandomTests(int testCount) override;

	std::pair<std::string, int> printAllData() override;

	void clear() override;

	~DiskDatabase() = default;
};