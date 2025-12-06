#include "../../../../Headers/Model/PCRDatabase/Generator/RandomDataGenerator.h"


int RandomDataGenerator::s_personId = 1;
unsigned int RandomDataGenerator::s_testId = 1;
std::vector<PersonHashWrapper*> RandomDataGenerator::s_people{};

std::chrono::year_month_day RandomDataGenerator::generateRandomDate(std::mt19937& generator)
{
    using namespace std::chrono;

    year_month_day start{ year{MIN_YEAR}, month{MIN_MONTH}, day{MIN_DAY} };
    year_month_day end{ year{MAX_YEAR}, month{MAX_MONTH}, day{MAX_DAY} };

    sys_days startDays = sys_days(start);
    sys_days endDays = sys_days(end);

    std::uniform_int_distribution<int> dist(0, (endDays - startDays).count());
    sys_days randomDay = startDays + days(dist(generator));

    return year_month_day{ randomDay };
}

std::chrono::time_point<std::chrono::system_clock> RandomDataGenerator::generateTime(std::mt19937& generator, const std::chrono::year_month_day& birthDate)
{
    using namespace std::chrono;

    const year_month_day start = birthDate + years{ MIN_AGE };

    //year_month_day maxDateTime{ year{MAX_TEST_YEAR}, month{MAX_TEST_MONTH}, day{MAX_TEST_DAY} };
    //sys_days maxDay = sys_days(maxDateTime);

    const auto max = system_clock::now();       //maxDay + hours(23) + minutes(59) + seconds(59)
    const auto currentYmd = floor<days>(max);
    const year_month_day end{ currentYmd };

    if (end < start) {
        return system_clock::now();
    }

    sys_days startDays = sys_days(start);
    sys_days endDays = sys_days(end);

    std::uniform_int_distribution<int> dist(0, (endDays - startDays).count());
    sys_days randomDay = startDays + days(dist(generator));

    std::uniform_int_distribution<int> hour_dist(0, 23);
    std::uniform_int_distribution<int> min_dist(0, 59);
    std::uniform_int_distribution<int> sec_dist(0, 59);

    auto randomTime = hours(hour_dist(generator)) +
                      minutes(min_dist(generator)) +
                      seconds(sec_dist(generator));

    return randomDay + randomTime;
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
       << std::setw(4) << std::setfill('0') << std::to_string(birthNumberInterval(generator));

    return pn.str();
}

void RandomDataGenerator::generateLocation(std::mt19937& generator, unsigned int& workplace, unsigned int& district, unsigned int& region)
{
    std::uniform_int_distribution<unsigned int> workplaceInterval(0, MAX_WORKPLACE_CODE - 1);

    workplace = workplaceInterval(generator);
    district = (workplace * MAX_DISTRICT_CODE) / MAX_WORKPLACE_CODE;
    region = (district * MAX_REGION_CODE) / MAX_DISTRICT_CODE;
}

void RandomDataGenerator::clearGeneratedPeople()
{
    for (auto& person : s_people)
    {
        delete person;
    }
    s_people.clear();
}

PersonHashWrapper* RandomDataGenerator::generatePerson(std::mt19937& gen)
{
    std::uniform_int_distribution<unsigned int> names(0, NAMES_COUNT - 1);

    auto birthDay = generateRandomDate(gen);
    std::string name(s_names[names(gen)]);
    std::string lastName(s_lastNames[names(gen)]);
    std::string birthNumber = std::to_string(s_personId);
    ++s_personId;

    PersonHashWrapper* person = new PersonHashWrapper(new Person(birthNumber, name, lastName, birthDay));
    s_people.push_back(person);

    return person;
}

TestHashWrapper* RandomDataGenerator::generateTest(std::mt19937& gen)
{
    std::uniform_int_distribution<unsigned int> noteInterval(0, NOTE_COUNT - 1);
    std::uniform_int_distribution<unsigned int> resultInterval(0, 1);
    std::uniform_real_distribution<double> valueInterval(10.0, 40.0);
    std::uniform_int_distribution<unsigned int> randomPersonInterval(0, s_people.size() - 1);

    Person* person = s_people[randomPersonInterval(gen)]->getData();
    bool result = static_cast<bool>(resultInterval(gen));
    std::string note(s_notes[noteInterval(gen)]);
    unsigned int id = s_testId;
    ++s_testId;

    return new TestHashWrapper(new ReducedPCRTest(
        id,
        result,
        result ? valueInterval(gen) : 0,
        note,
        generateTime(gen, person->birthDay())
    ), person->birthNumber());
}

PersonWrapper* RandomDataGenerator::generatePeople(std::vector<PersonWrapper*>& peopleDuplicityList, AVLTree<PersonWrapper*>& people)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    PersonHashWrapper* personHashWrapper = generatePerson(gen);
    Person* person = personHashWrapper->getData();
    PersonWrapper key(person);

    while (people.find(&key) != nullptr)
    {
        auto birthDay = person->birthDay();
        person->setBirthNumber(generateBirthNumber(gen, birthDay));
    }

    delete personHashWrapper;

   return new PersonWrapper(person);
}

TestWrapper* RandomDataGenerator::generateTests(std::vector<PersonWrapper*>& peopleDuplicityList, AVLTree<TestWrapper*>& tests)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<unsigned int> noteInterval(0, NOTE_COUNT - 1);
    std::uniform_int_distribution<unsigned int> testIdInterval(0, MAX_TEST_CODE - 1);
    std::uniform_int_distribution<unsigned int> resultInterval(0, 1);
    std::uniform_real_distribution<double> valueInterval(10.0, 40.0);
    std::uniform_int_distribution<unsigned int> randomPersonInterval(0, peopleDuplicityList.size() - 1);

    PersonWrapper* correspondingPerson = peopleDuplicityList[randomPersonInterval(gen)];
    unsigned int workplace, district, region;
    generateLocation(gen, workplace, district, region);
    std::string note(s_notes[noteInterval(gen)]);
    bool result = (bool)resultInterval(gen);
    double testValue = result ? valueInterval(gen) : 0;
    auto date = generateTime(gen, correspondingPerson->getData()->birthDay());
    unsigned int testId = testIdInterval(gen);

    PCRTest duplicityTest(
        testId,
        workplace,
        district,
        region,
        result,
        testValue,
        note,
        date,
        correspondingPerson->getData()->birthNumber()
    );
    TestWrapper key(&duplicityTest);

    while (tests.find(&key) != nullptr)
    {
        duplicityTest.setTestId(testIdInterval(gen));
    }

    return new TestWrapper(new PCRTest(
        duplicityTest.testId(),
        workplace,
        district,
        region,
        result,
        testValue,
        note,
        date,
        correspondingPerson->getData()->birthNumber()
    ));
}
