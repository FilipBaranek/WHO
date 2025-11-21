#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <bitset>
#include "Headers/Model/Tests/FunctionalityTester.h"
#include "Headers/Model/Tests/SpeedTester.h"
#include "Headers/Model/Tests/FileTester.h"
#include "Headers/View/Application.h"
#include "Headers/Model/Factories/RecordFactory.h"


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

    FileTester<Person> tester("../../../data/", dummy->getSize() * 5);
    tester.runTests();

    delete dummy;
}

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    {
        testFile();
    }

    return 0;
}