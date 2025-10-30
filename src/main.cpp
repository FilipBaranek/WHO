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
        db.generateRandomData(5);
        db.printAllData();

        std::cout << db.findTestResultByIdAndPatientId(4567, "970211/4963") << "\n";
    }

    return 0;
}