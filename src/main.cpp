#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <bitset>
#include "Headers/Model/Factories/RecordFactory.h"
#include "Headers/Model/Tests/FunctionalityTester.h"
#include "Headers/Model/Tests/SpeedTester.h"
#include "Headers/Model/Tests/FileTester.h""
#include "Headers/Model/Tests/HashFileTester.h"
#include "Headers/Model/PCRDatabase/ModelWrappers/PersonHashWrapper.h"
#include "Headers/View/UI/SelectDatabase.h"


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
    PersonHashWrapper* dummy = RecordFactory::createInstance<PersonHashWrapper>();

    FileTester tester("../../../data/HeapFile/data", dummy->getSize() * 3);
    tester.runTests();

    delete dummy;
}

void testHashFile()
{
    PersonHashWrapper* dummy = RecordFactory::createInstance<PersonHashWrapper>();

    HashFileTester tester("../../../data/HashFile/", 4, dummy->getSize() * 8, dummy->getSize() * 3);
    tester.runTests();

    delete dummy;
}

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    {
        //testFile();
        //testHashFile();

        SelectDatabaseWindow mainApp;
        mainApp.run();
    }

    return 0;
}