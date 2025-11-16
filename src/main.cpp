#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <bitset>
#include "Headers/Model/Tests/FunctionalityTester.h"
#include "Headers/Model/Tests/SpeedTester.h"
#include "Headers/View/Application.h"

#include "Headers/Model/Factories/RecordFactory.h"
#include "Headers/Model/HeapFile/Block.h"

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

        //Person person{
        //    "01234",
        //    "Filip",
        //    "Nigga",
        //    std::chrono::year_month_day{std::chrono::year{2002}, std::chrono::month{11}, std::chrono::day{18}}
        //};
        //uint8_t byteBuffer[500];

        //std::cout << "Person before converting:" << person.toString() << "\n";
        //
        //person.toBytes(byteBuffer);
        //
        //std::cout << "Person in binary:\n";
        //for (int i{}; i < person.getSize(); ++i)
        //{
        //    std::cout << std::bitset<8>(byteBuffer[i]) << "\n";
        //}


        //Person* loadedPerson = RecordFactory::createInstance<Person>(byteBuffer);

        //std::cout << "\nPerson from bytes:" << loadedPerson->toString();
        //
        //delete loadedPerson;

		Person* person1 = new Person{
	        "01234",
	        "Filip",
	        "Nigga",
	        std::chrono::year_month_day{std::chrono::year{2002}, std::chrono::month{11}, std::chrono::day{18}}
		};

        Person* person2 = new Person{
            "01235",
            "Nigga",
            "Niggerska",
            std::chrono::year_month_day{std::chrono::year{1999}, std::chrono::month{11}, std::chrono::day{7}}
        };

        Person* person3 = new Person{
            "01236",
            "Tatko",
            "Average",
            std::chrono::year_month_day{std::chrono::year{1998}, std::chrono::month{6}, std::chrono::day{5}}
        };

        uint8_t buffer[1000];
        Block<Person> block(person1->getSize() * 3, person1->getSize());
        block.insert(person1);
        block.insert(person2);
        block.insert(person3);

        std::cout << "People in block";
        for (auto& person : block.objects())
        {
            std::cout << person->toString() << "\n";
        }

        block.toBytes(buffer);
        std::cout << "People in binary:\n";
        for (int i{}; i < person1->getSize() * 3; ++i)
        {
            std::cout << std::bitset<8>(buffer[i]) << "\n";
        }

        block.fromBytes(buffer);
        std::cout << "People composed from binary";
        for (auto& person : block.objects())
        {
            std::cout << person->toString() << "\n";
        }

    }

    return 0;
}