#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <chrono>
#include "../Structures/AVL/AVLTree.h"
#include "./Generator/RandomDataGenerator.h"
#include "./ModelWrappers/PersonWrapper.h"
#include "./ModelWrappers/LocationWrapper.h"
#include "./ModelWrappers/TestByDateWrapper.h"


using namespace std::chrono;

class Database
{
private:
	static constexpr const int MIN_ID = 0;
	static constexpr const int MAX_ID = 99999999999999;
	static constexpr const char* MIN_STRING_VAL = "\0";
	static constexpr const char* MAX_STRING_VAL = "zzzzzzzzzzzzzzzzzzzz\0";
	static constexpr const time_point<system_clock> MIN_TIME_POINT = time_point<system_clock>::min();
	static constexpr const time_point<system_clock> MAX_TIME_POINT = time_point<system_clock>::max();

	static constexpr const int DEFAULT_NUM_VAL = 0;
	static constexpr const bool DEFAULT_BOOL_VAL = false;
	static constexpr const char* DEFAULT_STRING_VAL = "\0";
	static constexpr const time_point<system_clock> DEFAULT_TIME_POINT{};
	static constexpr const year_month_day DEFAULT_DATE{};

	std::vector<PersonWrapper*> m_peopleList;
	AVLTree<PersonWrapper*> m_people;

	std::vector<AVLTree<LocationWrapper*>*> m_locationStructures;
	AVLTree<LocationWrapper*> m_districts;
	AVLTree<LocationWrapper*> m_regions;
	AVLTree<LocationWrapper*> m_workplaces;

	std::pair<AVLTree<TestByDateWrapper*>*, AVLTree<TestByDateWrapper*>*> m_testStructuresList;
	AVLTree<TestByDateWrapper*> m_positiveTests;
	AVLTree<TestByDateWrapper*> m_negativeTests;

	std::pair<std::string, int> outputToString(std::vector<TestByDateWrapper*>& output);
	void sickPeopleInDistrict(const unsigned int& districtId, time_point<system_clock>& from,
							  time_point<system_clock>& to, LocationWrapper*& foundLocation, std::vector<TestByDateWrapper*>& output);

public:
	Database();
	
	void generateRandomPeople(int peopleCount);
	
	bool generateRandomTests(int testCount);
	
	PersonWrapper* findPerson(std::string birthNumber);

	//(1)
	bool insert(TestByDateWrapper* pcrTest);
	
	//(2)
	std::string findTestResultByIdAndPatientId(const unsigned int testId, const std::string birthBumber, bool printPerson = true);
	
	//(3)
	std::pair<std::string, int> findPatientTestOrderByDate(std::string birthNumber);

	//(4)
	std::pair<std::string, int> findPositiveTestsInDistrict(const unsigned int districtId, time_point<system_clock> from, time_point<system_clock> to);

	//(5)
	std::pair<std::string, int> findAllTestsInDistrict(const unsigned int districtId, time_point<system_clock> from, time_point<system_clock> to);

	//(6)
	std::pair<std::string, int> findPositiveTestsInRegion(const unsigned int regionId, time_point<system_clock> from, time_point<system_clock> to);

	//(7)
	std::pair<std::string, int> findAllTestsInRegion(const unsigned int regionId, time_point<system_clock> from, time_point<system_clock> to);

	//(8)
	std::pair<std::string, int> findPositiveTests(time_point<system_clock> from, time_point<system_clock> to);

	//(9)
	std::pair<std::string, int> findAllTests(time_point<system_clock> from, time_point<system_clock> to);

	//(10)
	std::pair<std::string, int> findSickPeopleInDistrict(const unsigned int districtId, time_point<system_clock> from, time_point<system_clock> to);

	//(11)
	std::pair<std::string, int> findSickPeopleInDistrictOrderedByTestValue(const unsigned int districtId, time_point<system_clock> from, time_point<system_clock> to);

	//(12)
	std::pair<std::string, int> findSickPeopleInRegion(const unsigned int regionId, time_point<system_clock> from, time_point<system_clock> to);

	//(13)
	std::pair<std::string, int> findSickPeople(time_point<system_clock> from, time_point<system_clock> to);

	//(14)
	std::pair<std::string, int> findMostSickPersonInDistrict(time_point<system_clock> from, time_point<system_clock> to);
	
	//(15)
	std::pair<std::string, int> findDistrictsOrderedBySickPeopleCount(time_point<system_clock> from, time_point<system_clock> to);

	//(16)
	std::pair<std::string, int> findRegionsOrderedBySickPeopleCount(time_point<system_clock> from, time_point<system_clock> to);

	//(17)
	std::pair<std::string, int> findAllTestsAtWorkplace(int workplaceId, time_point<system_clock> from, time_point<system_clock> to);

	//(18)
	std::string findTest(const unsigned int testId, bool printPerson);

	//(19)
	bool insert(PersonWrapper* person);
	
	//(20)
	int removeTest(int testId);
	
	//(21)
	int removePerson(std::string birthNumber);
	
	std::pair<std::string, int> printAllData();
	
	void clear();
	
	~Database();
};