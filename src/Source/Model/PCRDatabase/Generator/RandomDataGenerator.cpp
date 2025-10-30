#include "../../../../Headers/Model/PCRDatabase/Generator/RandomDataGenerator.h"

std::chrono::year_month_day RandomDataGenerator::generateRandomDate(std::mt19937& generator)
{
    using namespace std::chrono;

    year_month_day start{ year{1980}, month{1}, day{1} };
    year_month_day end{ year{2010}, month{12}, day{31} };

    sys_days start_days = sys_days(start);
    sys_days end_days = sys_days(end);

    std::uniform_int_distribution<int> dist(0, (end_days - start_days).count());
    sys_days random_day = start_days + days(dist(generator));

    return year_month_day{ random_day };
}

std::chrono::time_point<std::chrono::system_clock> RandomDataGenerator::generateTime(std::mt19937& generator, const std::chrono::year_month_day& birthDate)
{
    using namespace std::chrono;

    const year_month_day start = birthDate + years{ 15 };

    const auto now = std::chrono::system_clock::now();
    const auto current_ymd = floor<days>(now);
    const year_month_day end{ current_ymd };

    if (end < start) {
        return std::chrono::system_clock::now();
    }

    sys_days start_days = sys_days(start);
    sys_days end_days = sys_days(end);

    std::uniform_int_distribution<int> dist(0, (end_days - start_days).count());
    sys_days random_day = start_days + days(dist(generator));

    std::uniform_int_distribution<int> hour_dist(0, 23);
    std::uniform_int_distribution<int> min_dist(0, 59);
    std::uniform_int_distribution<int> sec_dist(0, 59);

    auto random_time = std::chrono::hours(hour_dist(generator)) +
        std::chrono::minutes(min_dist(generator)) +
        std::chrono::seconds(sec_dist(generator));

    return random_day + random_time;
}

std::string RandomDataGenerator::generateBirthNumber(std::mt19937& generator, std::chrono::year_month_day& birthDay)
{
    using namespace std::chrono;

    std::uniform_int_distribution<unsigned int> birthNumberInterval(0, MAX_BIRTH_NUMBER - 1);

    int year = static_cast<int>(birthDay.year()) % 100;
    int month = static_cast<unsigned>(birthDay.month());
    int day = static_cast<unsigned>(birthDay.day());

    std::ostringstream pn;
    pn << std::setw(2) << std::setfill('0') << year
       << std::setw(2) << std::setfill('0') << month
       << std::setw(2) << std::setfill('0') << day
       << "/" << std::setw(3) << std::setfill('0') << std::to_string(birthNumberInterval(generator));

    return pn.str();
}

void RandomDataGenerator::generateLocation(std::mt19937& generator, unsigned int& workplace, unsigned int& district, unsigned int& region)
{
    std::uniform_int_distribution<unsigned int> workplaceInterval(0, MAX_WORKPLACE_CODE - 1);

    workplace = workplaceInterval(generator);
    district = (workplace * MAX_DISTRICT_CODE) / MAX_WORKPLACE_CODE;
    region = (district * MAX_REGION_CODE) / MAX_DISTRICT_CODE;
}

void RandomDataGenerator::generatePeople(std::vector<Person*>& output)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<unsigned int> names(0, NAMES_COUNT - 1);

    auto birthDay = generateRandomDate(gen);
    std::string name(s_names[names(gen)]);
    std::string lastName(s_lastNames[names(gen)]);
    std::string birthNumber = generateBirthNumber(gen, birthDay);

    while (std::find_if(output.begin(), output.end(), [&](Person* person) {
        return birthNumber == person->birthNumber();
    }) != output.end())
    {
        birthNumber = generateBirthNumber(gen, birthDay);
    }


	output.push_back(new Person(birthNumber, name, lastName, birthDay));
}

void RandomDataGenerator::generateTests(AVLTree<PersonWrapper*>& input, std::vector<PCRTest*>& output)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<unsigned int> noteInterval(0, NOTE_COUNT - 1);
    std::uniform_int_distribution<unsigned int> test(0, MAX_TEST_CODE - 1);
    std::uniform_int_distribution<unsigned int> result(0, 1);
    std::uniform_int_distribution<unsigned int> value(10, 40);
    std::uniform_int_distribution<unsigned int> birthNumber(0, input.size() - 1);

    std::string note(s_notes[noteInterval(gen)]);
    unsigned int testId = test(gen);

    while (std::find_if(output.begin(), output.end(), [&](PCRTest* test) {
        return testId == test->testId();
    }) != output.end())
    {
        testId = test(gen);
    }

    Person* randomPerson;
    bool stopExecuting = false;
    int i = 0;
    int randomPersonIndex = birthNumber(gen);

    //Might be optimized needed later
    input.processInOrder([&i, &randomPersonIndex, &randomPerson, &stopExecuting](PersonWrapper* person) {
        if (stopExecuting)
        {
            return;
        }
        if (i == randomPersonIndex)
        {
            randomPerson = person->getData();
            stopExecuting = true;
            return;
        }
        ++i;
    });

    unsigned int workplace, district, region;
    generateLocation(gen, workplace, district, region);

    output.push_back(new PCRTest(
        testId,
        workplace,
        district,
        region,
        (bool)result(gen),
        value(gen),
        note,
        generateTime(gen, randomPerson->birthDay()),
        randomPerson->birthNumber(),
        randomPerson
    ));
}
