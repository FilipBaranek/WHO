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

void RandomDataGenerator::generatePeople(std::vector<PersonWrapper*>& peopleDuplicityList, AVLTree<PersonWrapper*>& output)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<unsigned int> names(0, NAMES_COUNT - 1);

    auto birthDay = generateRandomDate(gen);
    std::string name(s_names[names(gen)]);
    std::string lastName(s_lastNames[names(gen)]);
    std::string birthNumber = generateBirthNumber(gen, birthDay);

    Person duplicitPerson(birthNumber, name, lastName, birthDay);
    PersonWrapper key(&duplicitPerson);

    while (output.find(&key) != nullptr)
    {
        duplicitPerson.setBirthNumber(generateBirthNumber(gen, birthDay));
    }

    PersonWrapper* newPerson = new PersonWrapper(new Person(duplicitPerson.birthNumber(), name, lastName, birthDay));
	output.insert(newPerson);
    peopleDuplicityList.push_back(newPerson);
}

void RandomDataGenerator::generateTests(std::vector<PersonWrapper*>& peopleList, std::vector<AVLTree<TestWrapper*>*>& outputStrucutres)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<unsigned int> noteInterval(0, NOTE_COUNT - 1);
    std::uniform_int_distribution<unsigned int> testIdInterval(0, MAX_TEST_CODE - 1);
    std::uniform_int_distribution<unsigned int> resultInterval(0, 1);
    std::uniform_real_distribution<double> valueInterval(10.0, 40.0);
    std::uniform_int_distribution<unsigned int> randomPersonInterval(0, peopleList.size() - 1);

    unsigned int workplace, district, region;
    generateLocation(gen, workplace, district, region);
    std::string note(s_notes[noteInterval(gen)]);
    PersonWrapper* correspondingPerson = peopleList[randomPersonInterval(gen)];
    bool result = (bool)resultInterval(gen);
    double testValue = valueInterval(gen);
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
        correspondingPerson->getData()->birthNumber(),
        correspondingPerson->getData()
    );
    TestWrapper key(&duplicityTest);

    while (outputStrucutres.at(0)->find(&key) != nullptr)
    {
        duplicityTest.setTestId(testIdInterval(gen));
    }

    auto newTest = new PCRTest(
        duplicityTest.testId(),
        workplace,
        district,
        region,
        result,
        testValue,
        note,
        date,
        correspondingPerson->getData()->birthNumber(),
        correspondingPerson->getData()
    );

    correspondingPerson->tests().insert(new TestByDateWrapper(newTest, correspondingPerson));
    outputStrucutres.at(0)->insert(new TestWrapper(newTest, correspondingPerson));

    if (result)
    {
        outputStrucutres.at(1)->insert(new TestInDistrictWrapper(newTest, correspondingPerson));
        outputStrucutres.at(3)->insert(new TestInRegionWrapper(newTest, correspondingPerson));
        outputStrucutres.at(5)->insert(new TestByDateWrapper(newTest, correspondingPerson));
    }
    else
    {
        outputStrucutres.at(2)->insert(new TestInDistrictWrapper(newTest, correspondingPerson));
        outputStrucutres.at(4)->insert(new TestInRegionWrapper(newTest, correspondingPerson));
        outputStrucutres.at(6)->insert(new TestByDateWrapper(newTest, correspondingPerson));
    }
}
