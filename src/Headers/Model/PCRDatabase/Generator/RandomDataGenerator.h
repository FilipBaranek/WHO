#pragma once
#include <string_view>
#include <vector>
#include <chrono>
#include <random>
#include "../Models/PCRTest.h"
#include "../Models/Person.h"


class RandomDataGenerator
{
private:
	//Person
	static constexpr const unsigned int MAX_BIRTH_NUMBER = 10'000;
	static constexpr const unsigned int NAMES_COUNT = 5;
	static constexpr std::string_view s_names[] = { "Peter", "Jana", "Martin", "Lucia", "Tomas" };
	static constexpr std::string_view s_lastNames[] = { "Novak", "Horvath", "Kovac", "Bielik", "Simko" };

	//Test
	static constexpr const unsigned int MAX_TEST_CODE = 1'000'000;
	static constexpr const unsigned int MAX_WORKPLACE_CODE = 100;
	static constexpr const unsigned int MAX_DISTRICT_CODE = 79;
	static constexpr const unsigned int MAX_REGION_CODE = 8;
	static constexpr const unsigned int NOTE_COUNT = 3;
	static constexpr std::string_view s_notes[] = { "Pacient v karantene", "Vysledok zaslany elektronicky", "Opakovany test odporucany" };

	static std::chrono::year_month_day generateRandomDate();
	static std::chrono::time_point<std::chrono::system_clock> generateTime();
	static std::string generateBirthNumber(std::chrono::year_month_day& birthDay);

public:
	RandomDataGenerator() = delete;
	static void generatePeople(std::vector<Person*>& output);
	static void generateTests(std::vector<Person*>& input, std::vector<PCRTest*>& output);
	~RandomDataGenerator() = delete;
};