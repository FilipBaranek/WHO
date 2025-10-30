#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include "./Headers/Model/Tests/FunctionalityTester.h"
#include "./Headers/Model/Tests/SpeedTester.h"
#include "./Headers/View/Application.h"

#include "./Headers/Model/PCRDatabase/Database.h"


void testSpeed()
{
    bool dataInOrder = false;

    SpeedTester tester(dataInOrder);
    tester.testInsertion();
    tester.testRemoval();
    tester.testPointSearch();
    tester.testIntervalSearch();
    tester.testFindMinKey();
    tester.testFindMaxKey();

    std::cout << tester.outputToString() << "\n";
}

void testFunctionality()
{
    unsigned int pregeneratedDataCount = 10000;

    FunctionalityTester tester(pregeneratedDataCount);
    tester.runTests();

    std::cout << tester.outputToString() << "\n";
}

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    {
        //testFunctionality();
        //testSpeed();

        //Application app;
        //app.run();

        Database db;
        db.generateRandomPeople(8);
        db.generateRandomTests(20);
        db.printAllData();



        //db.insert(new PCRTest(136502, 38, 30, 3, 1, 14, "", std::chrono::system_clock::now(), "850119/7714"));
        //db.insert(new PCRTest(254865, 38, 30, 3, 1, 14, "", std::chrono::system_clock::now(), "940318/8575"));
        //db.insert(new PCRTest(844101, 38, 30, 3, 1, 14, "", std::chrono::system_clock::now(), "940318/8575"));
        //db.insert(new PCRTest(629873, 38, 30, 3, 1, 14, "", std::chrono::system_clock::now(), "980518/1020"));
        //db.insert(new PCRTest(840310, 38, 30, 3, 1, 14, "", std::chrono::system_clock::now(), "980518/1020"));
    }

    return 0;
}