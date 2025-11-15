#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <bitset>
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
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    {
        //testFunctionality();
        //testSpeed(false);

        //Application app;
        //app.run();

        Person person{
            "01234",
            "Filip",
            "Nigga",
            std::chrono::year_month_day{std::chrono::year{2002}, std::chrono::month{11}, std::chrono::day{18}}
        };
        std::vector<uint8_t> byteBuffer;

        std::cout << "Person before converting:" << person.toString() << "\n";
        
        person.toBytes(byteBuffer);
        
        std::cout << "Person in binary:\n";
        for (auto& byte : byteBuffer)
        {
            std::cout << std::bitset<8>(byte) << "\n";
        }

        Person* loadedPerson = static_cast<Person*>(person.fromBytes(byteBuffer.data()));

        std::cout << "\nPerson from bytes:" << loadedPerson->toString();
        
        delete loadedPerson;
    }

    return 0;
}