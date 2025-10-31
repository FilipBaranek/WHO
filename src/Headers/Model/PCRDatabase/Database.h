#pragma once
#include <string>
#include <sstream>
#include <vector>
#include "./ModelWrappers/TestWrapper.h"
#include "./ModelWrappers/PersonWrapper.h"
#include "../Structures/AVL/AVLTree.h"
#include "./Generator/RandomDataGenerator.h"


class Database
{
private:
	static constexpr const int MIN_TEST_ID = 0;
	static constexpr const int MAX_TEST_ID = 99999999999999;
	static constexpr const int DEFAULT_NUM_VAL = 0;
	static constexpr const bool DEFAULT_BOOL_VAL = false;
	static constexpr const char* DEFAULT_STRING_VAL = "\0";
	static constexpr const std::chrono::time_point <std::chrono::system_clock> DEFAULT_TIME_POINT{};
	static constexpr const std::chrono::year_month_day DEFAULT_DATE{};
	
	std::ostringstream m_oss;

	std::vector<Person*> m_peopleList;
	AVLTree<PersonWrapper*> m_people;

	std::vector<AVLTree<TestWrapper*>*> m_testStructuresList;
	AVLTree<TestWrapper*> m_tests;

public:
	Database();
	void generateRandomPeople(int peopleCount);
	void generateRandomTests(int testCount);
	void insert(Person* person);
	void insert(PCRTest* pcrTest);
	std::string findTestResultByIdAndPatientId(const unsigned int testId, const std::string birthBumber);
	void printAllData();
	void clear();
	~Database();
};