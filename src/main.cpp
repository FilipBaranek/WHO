#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include "./Headers/Model/Tests/FunctionalityTester.h"
#include "./Headers/Model/Tests/SpeedTester.h"
#include "./Headers/View/Application.h"
#include "./Headers/Presenter/Presenter.h"


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

int main()
{
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    {
        //testFunctionality();
        //testSpeed(false);

        //Application app;
        //app.run();
    }

    //testSpeed(false);

    Application app;
    app.run();

    return 0;
}