#pragma once
#include "Database.h"
#include "ModelWrappers/PersonHashWrapper.h"
#include "ModelWrappers/TestHashWrapper.h"
#include "Generator/RandomDataGenerator.h"
#include "../HashFile/HashFile.h"
#include "../../../Headers/View/Models/Person.h"
#include "../../../Headers/View/Models/Test.h"

class DiskDatabase : public Database
{
private:
	static constexpr const char* FILE_PATH = "../../../data/HashFile/";

	std::random_device m_rd;
	std::mt19937 m_gen{ 1928130314 };

	HashFile<PersonHashWrapper> m_people;
	HashFile<TestHashWrapper> m_tests;

private:
	bool updatePersonTests(std::string birthBumber, unsigned int testId);

public:
	DiskDatabase(std::string filePath, int peopleGroupSize, int peoplePrimaryClusterSize, int peopleOverflowClusterSize,
				 int testsGroupSize, int testsPrimaryClusterSize, int testsOverflowClusterSize);

	void generateRandomPeople(int peopleCount) override;

	//(1)
	void insert(TestHashWrapper* test);

	//(2)
	std::pair<std::string, int> findPerson(std::string birthNumber);

	//(3)
	std::string findTest(const unsigned int testId) override;

	//(4)
	void insert(PersonHashWrapper* person);

	//(7)
	std::pair<PersonView*, std::string> findPersonToEdit(std::string birthNumber);

	bool editPerson(Person* personToEdit);

	//(8)
	std::pair<TestView*, std::string> findTestToEdit(const unsigned int testId);

	bool editTest(ReducedPCRTest* testToEdit);

	std::pair<std::string, int> printAllData() override;

	void clear() override;

	int size();

	~DiskDatabase();
};