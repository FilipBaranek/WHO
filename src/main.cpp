#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <bitset>
#include "Headers/Model/Tests/FunctionalityTester.h"
#include "Headers/Model/Tests/SpeedTester.h"
#include "Headers/Model/Tests/FileTester.h""
#include "Headers/Model/Tests/HashFileTester.h"
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

void testHashFile()
{
    Person* dummy = RecordFactory::createInstance<Person>();

    HashFileTester tester(dummy->getSize() * 2, dummy->getSize());
    tester.runTests();

    delete dummy;
}

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    {
        //testFile();
        testHashFile();

        //Application app(APPLICATIONTYPE::DISK);
        //app.run();


        //Person* dummy = RecordFactory::createInstance<Person>();
        //Person* person1 = new Person("zi", "27", "27", std::chrono::year_month_day{});
        //Person* person2 = new Person("v", "18", "18", std::chrono::year_month_day{});
        //Person* person3 = new Person("zzU", "29", "29", std::chrono::year_month_day{});
        //Person* person4 = new Person("zj", "28", "28", std::chrono::year_month_day{});
        //Person* person5 = new Person("zu", "39", "39", std::chrono::year_month_day{});
        //Person* person6 = new Person("q", "13", "13", std::chrono::year_month_day{});
        //Person* person7 = new Person("t", "16", "16", std::chrono::year_month_day{});
        //Person* person8 = new Person("3", "51", "51", std::chrono::year_month_day{});
        //Person* person9 = new Person("w", "19", "19", std::chrono::year_month_day{});

        //HashFile<Person> hashFile(dummy->getSize() * 2, dummy->getSize());
        //hashFile.open();

        //hashFile.insert(person1);
        //hashFile.printOut();
        //hashFile.insert(person2);
        //hashFile.printOut();
        //hashFile.insert(person3);
        //hashFile.printOut();
        //hashFile.insert(person4);
        //hashFile.printOut();
        //hashFile.insert(person5);
        //hashFile.printOut();
        //hashFile.insert(person6);
        //hashFile.printOut();
        //hashFile.insert(person7);
        //hashFile.printOut();
        //hashFile.insert(person8);
        //hashFile.printOut();
        //hashFile.insert(person9);
        //hashFile.printOut();

        //std::cout << "FIND METHOD:\n";
        //Person* foundPerson = hashFile.find(person2);
        //if (foundPerson != nullptr)
        //{
        //    std::cout << foundPerson->toString();
        //    delete foundPerson;
        //}
        //Person* foundPerson2 = hashFile.find(person9);
        //if (foundPerson2 != nullptr)
        //{
        //    std::cout << foundPerson2->toString();
        //    delete foundPerson2;
        //}

        //hashFile.clear();

        //delete person1;
        //delete person2;
        //delete person3;
        //delete person4;
        //delete person5;
        //delete person6;
        //delete person7;
        //delete person8;
        //delete person9;
        //delete dummy;
    }

    return 0;
}