#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <bitset>
#include "Headers/Model/Tests/FunctionalityTester.h"
#include "Headers/Model/Tests/SpeedTester.h"
#include "Headers/View/Application.h"

#include "Headers/Model/Factories/RecordFactory.h"
#include "Headers/Model/HeapFile/Block.h"
#include "Headers/Model/HeapFile/HeapFile.h"

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

		Person* person4 = new Person{
			"01237",
			"asdawda",
			"Adsadsada",
			std::chrono::year_month_day{std::chrono::year{1998}, std::chrono::month{6}, std::chrono::day{5}}
		};

		Person* person5 = new Person{
			"01238",
			"Netatko",
			"NeAverage",
			std::chrono::year_month_day{std::chrono::year{1998}, std::chrono::month{6}, std::chrono::day{5}}
		};

		Person* person6 = new Person{
			"01239",
			"jj",
			"cc",
			std::chrono::year_month_day{std::chrono::year{1998}, std::chrono::month{6}, std::chrono::day{5}}
		};

		Person* person7 = new Person{
			"01240",
			"nn",
			"bb",
			std::chrono::year_month_day{std::chrono::year{1998}, std::chrono::month{6}, std::chrono::day{5}}
		};

		Person* person8 = new Person{
			"01240",
			"zz",
			"cc",
			std::chrono::year_month_day{std::chrono::year{1998}, std::chrono::month{6}, std::chrono::day{5}}
		};

        Person* dummy = RecordFactory::createInstance<Person>();

        HeapFile<Person> heapFile("../../../data/", dummy->getSize() * 5);
        heapFile.open();
        int addr1 = heapFile.insert(person1);
		std::cout << addr1 << "\n";
		int addr2 = heapFile.insert(person2);
		std::cout << addr2 << "\n";
		int addr3 = heapFile.insert(person3);
		std::cout << addr3 << "\n";
		
		Person key("01235", "", "", dummy->birthDay());
		
		Person* foundPerson = heapFile.find(addr2, &key);
		std::cout << foundPerson->toString();
		Person* removedPerson = heapFile.remove(addr2, &key);
		std::cout << heapFile.size() << "\n";

		if (removedPerson != nullptr)
		{
			std::cout << "Person was removed\n";
		}

		delete foundPerson;

		foundPerson = heapFile.find(addr2, &key);
		if (foundPerson == nullptr)
		{
			std::cout << "Person was realy removed\n";
		}
		else
		{
			delete foundPerson;
		}

		int addr4 = heapFile.insert(person4);
		std::cout << addr4 << "\n";
		int addr5 = heapFile.insert(person5);
		std::cout << addr5 << "\n";
 		int addr6 = heapFile.insert(person6);
		std::cout << addr6 << "\n";
		int addr7 = heapFile.insert(person7);
		std::cout << addr7 << "\n";
		int addr8 = heapFile.insert(person8);
		std::cout << addr8 << "\n";

		delete removedPerson;
        delete dummy;
    }

    return 0;
}