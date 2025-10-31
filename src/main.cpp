#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include "./Headers/Model/Tests/FunctionalityTester.h"
#include "./Headers/Model/Tests/SpeedTester.h"
#include "./Headers/View/Application.h"
#include "./Headers/Presenter/Presenter.h"


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
        
        std::string output;

        Presenter presenter(output);
        presenter.insert("01", "a", "b", {});
        presenter.insert("02", "a", "b", {});
        presenter.insert("03", "a", "b", {});
        presenter.insert("04", "a", "b", {});
        presenter.insert("05", "a", "b", {});

        presenter.insert(1, 0, 0, 0, 1, 0, "", std::chrono::system_clock::now(), "03");
        presenter.insert(2, 0, 0, 0, 0, 0, "", std::chrono::system_clock::now(), "03");
        presenter.insert(3, 0, 0, 0, 0, 0, "", std::chrono::system_clock::now(), "02");

        presenter.findResultByPatientAndTestId(2, "03", true);

        std::cout << output << "\n";
    }

    return 0;
}