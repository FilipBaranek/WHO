#pragma once
#include <string_view>
#include <vector>
#include <chrono>
#include <random>
#include <utility>
#include "../Models/PCRTest.h"
#include "../Models/Person.h"
#include "../ModelWrappers/PersonWrapper.h"
#include "../ModelWrappers/LocationWrapper.h"
#include "../ModelWrappers/TestWrapper.h"
#include "../ModelWrappers/TestByDateWrapper.h"
#include "../ModelWrappers/PersonHashWrapper.h"
#include "../ModelWrappers/TestHashWrapper.h"
#include "../../Structures/AVL/AVLTree.h"


class RandomDataGenerator
{
private:
	//Date
	static constexpr const unsigned int MIN_YEAR = 1970;
	static constexpr const unsigned int MAX_YEAR = 2010;
	static constexpr const unsigned int MIN_MONTH = 1;
	static constexpr const unsigned int MAX_MONTH = 12;
	static constexpr const unsigned int MIN_DAY = 1;
	static constexpr const unsigned int MAX_DAY = 30;
	static constexpr const unsigned int MAX_TEST_YEAR = 2025;
	static constexpr const unsigned int MAX_TEST_MONTH = 11;
	static constexpr const unsigned int MAX_TEST_DAY = 10;
	static constexpr const unsigned int MIN_AGE = 15;

	//Person
	static constexpr const unsigned int MAX_BIRTH_NUMBER = 10'000;
	static constexpr const unsigned int NAMES_COUNT = 5;
	static constexpr std::string_view s_names[] = { "Peter", "Jana", "Martin", "Lucia", "Tomas" };
	static constexpr std::string_view s_lastNames[] = { "Novak", "Horvath", "Kovac", "Bielik", "Simko" };
	static int s_personId;

	//Test
	static constexpr const unsigned int MAX_TEST_CODE = 1'000'000;
	static constexpr const unsigned int MAX_WORKPLACE_CODE = 158;
	static constexpr const unsigned int MAX_DISTRICT_CODE = 79;
	static constexpr const unsigned int MAX_REGION_CODE = 8;
	static constexpr const double MAX_TEST_VALUE = 40.0;
	static constexpr const unsigned int NOTE_COUNT = 4;
	static constexpr std::string_view s_notes[] = { "", "Karantena", "Elektronic.", "Zopakovat" };
	static unsigned int s_testId;

	//Registered people
	static std::vector<PersonHashWrapper*> s_people;

	static std::chrono::year_month_day generateRandomDate(std::mt19937& generator);
	static std::chrono::time_point<std::chrono::system_clock> generateTime(std::mt19937& generator, const std::chrono::year_month_day& birthDate);
	static std::string generateBirthNumber(std::mt19937& generator, std::chrono::year_month_day& birthDay);
	static void generateLocation(std::mt19937& generator, unsigned int& workplace, unsigned int& district, unsigned int& region);

public:
	static void clearGeneratedPeople();

	inline static void initPersonId(int startingIndex) { s_personId = startingIndex; }

	inline static void initTestId(int startingIndex) { s_testId = startingIndex; }

	static PersonHashWrapper* generatePerson(std::mt19937& gen);

	static TestHashWrapper* generateTest(std::mt19937& gen);

	static PersonWrapper* generatePeople(std::vector<PersonWrapper*>& peopleDuplicityList, AVLTree<PersonWrapper*>& people);
	
	static TestWrapper* generateTests(std::vector<PersonWrapper*>& peopleList, AVLTree<TestWrapper*>& tests);

	RandomDataGenerator() = delete;
	~RandomDataGenerator() = delete;
};