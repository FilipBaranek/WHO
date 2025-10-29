#include "../../../../Headers/Model/PCRDatabase/Generator/RandomDataGenerator.h"

std::chrono::year_month_day RandomDataGenerator::generateRandomDate()
{
    using namespace std::chrono;

    static std::random_device rd;
    static std::mt19937 gen(rd());

    year_month_day start{ year{1980}, month{1}, day{1} };
    year_month_day end{ year{2010}, month{12}, day{31} };

    sys_days start_days = sys_days(start);
    sys_days end_days = sys_days(end);

    std::uniform_int_distribution<int> dist(0, (end_days - start_days).count());
    sys_days random_day = start_days + days(dist(gen));

    return year_month_day{ random_day };
}

std::chrono::time_point<std::chrono::system_clock> RandomDataGenerator::generateTime()
{
    auto ymd = generateRandomDate();
    auto random_day = std::chrono::sys_days(ymd);

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> hour_dist(0, 23);
    std::uniform_int_distribution<int> min_dist(0, 59);
    std::uniform_int_distribution<int> sec_dist(0, 59);

    auto random_time = std::chrono::hours(hour_dist(gen))
        + std::chrono::minutes(min_dist(gen))
        + std::chrono::seconds(sec_dist(gen));

    return std::chrono::time_point_cast<std::chrono::system_clock::duration>(random_day + random_time);
}

std::string RandomDataGenerator::generateBirthNumber(std::chrono::year_month_day& birthDay)
{
    using namespace std::chrono;

    int year = static_cast<int>(birthDay.year()) % 100;
    int month = static_cast<unsigned>(birthDay.month());
    int day = static_cast<unsigned>(birthDay.day());

    std::ostringstream pn;
    pn << std::setw(2) << std::setfill('0') << year
        << std::setw(2) << std::setfill('0') << month
        << std::setw(2) << std::setfill('0') << day
        << "/" << std::to_string(rand() % MAX_BIRTH_NUMBER);

    return pn.str();
}

void RandomDataGenerator::generatePeople(std::vector<Person*>& output)
{
    srand(time(nullptr));

    auto birthDay = generateRandomDate();
    std::string name(s_names[rand() % NAMES_COUNT]);
    std::string lastName(s_lastNames[rand() % NAMES_COUNT]);

	output.push_back(new Person(generateBirthNumber(birthDay), name, lastName, birthDay));
}

void RandomDataGenerator::generateTests(std::vector<Person*>& input, std::vector<PCRTest*>& output)
{
    srand(time(nullptr));

    std::string note(s_notes[rand() % NOTE_COUNT]);


    output.push_back(new PCRTest(
        rand() % MAX_TEST_CODE,
        rand() % MAX_WORKPLACE_CODE,
        rand() % MAX_DISTRICT_CODE,
        rand() % MAX_REGION_CODE,
        rand() % 2,
        (rand() % 30) + 10,
        note,
        generateTime(),
        input[rand() % input.size()]->birthNumber()
    ));
}
