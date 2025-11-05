#include "../../../../Headers/Model/PCRDatabase/Generator/RandomDataGenerator.h"

std::chrono::year_month_day RandomDataGenerator::generateRandomDate(std::mt19937& generator)
{
    using namespace std::chrono;

    year_month_day start{ year{MIN_YEAR}, month{1}, day{1} };
    year_month_day end{ year{MAX_YEAR}, month{12}, day{31} };

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

    const auto now = std::chrono::system_clock::now();
    const auto currentYmd = floor<days>(now);
    const year_month_day end{ currentYmd };

    if (end < start) {
        return std::chrono::system_clock::now();
    }

    sys_days startDays = sys_days(start);
    sys_days endDays = sys_days(end);

    std::uniform_int_distribution<int> dist(0, (endDays - startDays).count());
    sys_days randomDay = startDays + days(dist(generator));

    std::uniform_int_distribution<int> hour_dist(0, 23);
    std::uniform_int_distribution<int> min_dist(0, 59);
    std::uniform_int_distribution<int> sec_dist(0, 59);

    auto randomTime = std::chrono::hours(hour_dist(generator)) +
                      std::chrono::minutes(min_dist(generator)) +
                      std::chrono::seconds(sec_dist(generator));

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
       << "/" << std::setw(4) << std::setfill('0') << std::to_string(birthNumberInterval(generator));

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

void RandomDataGenerator::generateTests(std::vector<PersonWrapper*>& peopleList,
                                        std::pair<AVLTree<TestByDateWrapper*>*, AVLTree<TestByDateWrapper*>*>& testStructures,
                                        AVLTree<TestWrapper*>& tests, std::vector<AVLTree<LocationWrapper*>*>& locationStructures)
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
        correspondingPerson->getData()->birthNumber()
    );
    TestWrapper key(&duplicityTest);

    while (tests.find(&key) != nullptr)
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
        correspondingPerson->getData()->birthNumber()
    );

    LocationWrapper* regionWrapper = new LocationWrapper(region);
    LocationWrapper* districtWrapper = new LocationWrapper(district);
    LocationWrapper* workplaceWrapper = new LocationWrapper(workplace);
    if (!locationStructures.at(LOCATIONS::REGION)->insert(regionWrapper))
    {
        LocationWrapper* foundRegion = locationStructures.at(LOCATIONS::REGION)->find(regionWrapper);
        delete regionWrapper;
        regionWrapper = foundRegion;
    }
    if (!locationStructures.at(LOCATIONS::DISTRICT)->insert(districtWrapper))
    {
        LocationWrapper* foundDistrict = locationStructures.at(LOCATIONS::DISTRICT)->find(districtWrapper);
        delete districtWrapper;
        districtWrapper = foundDistrict;
    }
    if (!locationStructures.at(LOCATIONS::WORKPLACE)->insert(workplaceWrapper))
    {
        LocationWrapper* foundWorkplace = locationStructures.at(LOCATIONS::WORKPLACE)->find(workplaceWrapper);
        delete workplaceWrapper;
        workplaceWrapper = foundWorkplace;
    }

    TestByDateWrapper* newTestByDateWrapper = new TestByDateWrapper(newTest, correspondingPerson);
    TestWrapper* newTestWrapper = new TestWrapper(newTest, correspondingPerson);

    !tests.insert(newTestWrapper);
    correspondingPerson->tests().insert(newTestByDateWrapper);
    if (result)
    {
        testStructures.first->insert(newTestByDateWrapper);
        regionWrapper->positiveTests().insert(newTestByDateWrapper);
        districtWrapper->positiveTests().insert(newTestByDateWrapper);
        workplaceWrapper->positiveTests().insert(newTestByDateWrapper);
    }
    else
    {
        testStructures.second->insert(newTestByDateWrapper);
        regionWrapper->negativeTests().insert(newTestByDateWrapper);
        districtWrapper->negativeTests().insert(newTestByDateWrapper);
        workplaceWrapper->negativeTests().insert(newTestByDateWrapper);
    }
}
