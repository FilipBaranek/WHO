#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <chrono>
#include "../Structures/AVL/AVLTree.h"
#include "./Generator/RandomDataGenerator.h"
#include "./ModelWrappers/PersonWrapper.h"
#include "./ModelWrappers/TestWrapper.h"
#include "./ModelWrappers/TestByDateWrapper.h"
#include "./ModelWrappers/TestInDistrictWrapper.h"


class Database
{
private:
	static constexpr const int MIN_ID = 0;
	static constexpr const int MAX_ID = 99999999999999;
	static constexpr const char* MIN_STRING_VAL = "\0";
	static constexpr const char* MAX_STRING_VAL = "zzzzzzzzzzzzzzzzzzzz\0";
	static constexpr const std::chrono::time_point<std::chrono::system_clock> MIN_TIME_POINT = std::chrono::time_point<std::chrono::system_clock>::min();
	static constexpr const std::chrono::time_point<std::chrono::system_clock> MAX_TIME_POINT = std::chrono::time_point<std::chrono::system_clock>::max();

	static constexpr const int DEFAULT_NUM_VAL = 0;
	static constexpr const bool DEFAULT_BOOL_VAL = false;
	static constexpr const char* DEFAULT_STRING_VAL = "\0";
	static constexpr const std::chrono::time_point <std::chrono::system_clock> DEFAULT_TIME_POINT{};
	static constexpr const std::chrono::year_month_day DEFAULT_DATE{};
	
	std::ostringstream m_oss;

	std::vector<PersonWrapper*> m_peopleList;
	AVLTree<PersonWrapper*> m_people;

	std::vector<AVLTree<TestWrapper*>*> m_testStructuresList;
	AVLTree<TestWrapper*> m_tests;
	AVLTree<TestWrapper*> m_positiveTestsInDistrict;
	AVLTree<TestWrapper*> m_negativeTestsInDistrict;

public:
	Database();
	
	void generateRandomPeople(int peopleCount);
	
	bool generateRandomTests(int testCount);
	
	bool insert(PersonWrapper* person);
	
	bool insert(TestWrapper* pcrTest);
	
	Person* findPerson(std::string birthNumber);
	
	std::string findTestResultByIdAndPatientId(const unsigned int testId, const std::string birthBumber, bool printPerson = true);
	
	std::pair<std::string, int> findPatientTestOrderByDate(std::string birthNumber);

	std::pair<std::string, int> findPositiveTestsInDistrict(const unsigned int districtId,
															std::chrono::time_point<std::chrono::system_clock> from,
															std::chrono::time_point<std::chrono::system_clock> to);

	std::pair<std::string, int> findAllTestsInDistrict(const unsigned int districtId,
															std::chrono::time_point<std::chrono::system_clock> from,
															std::chrono::time_point<std::chrono::system_clock> to);
	
	std::string findTest(const unsigned int testId, bool printPerson);
	
	int removeTest(int testId);
	
	std::pair<bool, int> removePerson(std::string birthNumber);
	
	std::pair<std::string, int> printAllData();
	
	void clear();
	
	~Database();
};