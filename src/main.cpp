#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <bitset>
#include "Headers/Model/Tests/FunctionalityTester.h"
#include "Headers/Model/Tests/SpeedTester.h"
#include "Headers/Model/Tests/FileTester.h"
#include "Headers/View/Application.h"
#include "Headers/Model/Factories/RecordFactory.h"

#include "Headers/Model/HashFile/HashFile.h"
#include "Headers/Model/PCRDatabase/Models/Person.h"


void testSpeed(bool dataInOrder)
{
    SpeedTester tester(dataInOrder);
    tester.testInsertion();
    tester.testRemoval();
    tester.testPointSearch();
    tester.testIntervalSearch();
    tester.testFindMinKey();
    tester.testFindMaxKey();
}

void testFunctionality()
{
    unsigned int pregeneratedDataCount = 10000;

    FunctionalityTester tester(pregeneratedDataCount);
    tester.runTests();
}

void testFile()
{
    Person* dummy = RecordFactory::createInstance<Person>();

    FileTester tester("../../../data/HeapFile/data", dummy->getSize() * 3);
    tester.runTests();

    delete dummy;
}

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    {
        //testFile();

        //Application app(APPLICATIONTYPE::DISK);
        //app.run();


        Person* dummy = RecordFactory::createInstance<Person>();
        HashFile<Person> hashFile(dummy->getSize() * 2, dummy->getSize());
        hashFile.open();

        Person* person1 = new Person("aaa...z", "28", "28", std::chrono::year_month_day{});
        Person* person2 = new Person("r", "18", "18", std::chrono::year_month_day{});
        Person* person3 = new Person("ac", "29", "29", std::chrono::year_month_day{});

        hashFile.insert(person1);
        hashFile.insert(person1);
        hashFile.insert(person1);

        delete person1;
        delete person2;
        delete person3;
        delete dummy;
    }

    return 0;
}