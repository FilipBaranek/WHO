#pragma once
#include <string>
#include <thread>
#include <mutex>
#include "../Model/PCRDatabase/Database.h"
#include "Enums/Location.h"
#include "Enums/RemoveType.h"
#include "Commands/FindTestResultCommand.h"
#include "Commands/InsertCommand.h"
#include "Commands/GenerateCommand.h"
#include "Commands/PrintAllDataCommand.h"
#include "Commands/FindTestCommand.h"
#include "Commands/RemoveCommand.h"
#include "Commands/FindPatientsTestsCommand.h"
#include "Commands/FindPositiveTestsByLocationIdCommand.h"
#include "Commands/FindAllTestsByLocationIdCommand.h"

using namespace std::chrono;

class Presenter
{
private:
	bool m_isExecuting;
	std::mutex m_outputMutex;

	Database m_database;

	FindTestResultCommand m_resultCommand;
	InsertCommand m_insertCommand;
	GenerateCommand m_generateCommand;
	PrintAllDataCommand m_printAllDataCommand;
	FindTestCommand m_findTestCommand;
	RemoveCommand m_removeCommand;
	FindPatientsTestsCommand m_findPatientsTestCommand;
	FindPositiveTestsByLocationIdCommand m_findPositiveTestsByLocationIdCommand;
	FindAllTestsByLocationIdCommand m_findAllTestsByLocationIdCommand;

	std::string m_output;
	std::string m_recordCount;

	void execute(std::function<void(std::string& output, std::string& recordCount)> callback);

public:
	Presenter() : m_resultCommand(&m_database), m_insertCommand(&m_database), m_generateCommand(&m_database), m_isExecuting(false),
				  m_printAllDataCommand(&m_database), m_removeCommand(&m_database), m_findTestCommand(&m_database), m_findPatientsTestCommand(&m_database),
				  m_findPositiveTestsByLocationIdCommand(&m_database), m_findAllTestsByLocationIdCommand(&m_database)
	{};
	
	inline bool isExecuting() { return m_isExecuting; }

	void setOutput(std::string output, std::string recordCount);
	
	std::pair<std::string, std::string> output();

	void generatePeople(int count);

	void generateTests(int count);

	//(1)
	void insert(unsigned int testId, unsigned int workplaceId, unsigned int districtId, 
				unsigned int regionId, bool result, double testValue, std::string note,
				time_point<system_clock> testDate, std::string birthNumber);

	//(2)
	void findResultByPatientAndTestId(unsigned int testId, std::string birthNumber, bool printPerson);

	//(3)
	void findPatientTestOrderByDate(std::string birthNumber);

	//(4)
	void findPositiveTestsInDistrict(unsigned int id, time_point<system_clock> from, time_point<system_clock> to);

	//(5)
	void findAllTestsInDistrict(unsigned int id, time_point<system_clock> from, time_point<system_clock> to);

	//(6)
	void findPositiveTestsInRegion(unsigned int id, time_point<system_clock> from, time_point<system_clock> to);

	//(7)
	void findAllTestsInRegion(unsigned int id, time_point<system_clock> from, time_point<system_clock> to);

	//(8)
	void findPositiveTests(time_point<system_clock> from, time_point<system_clock> to);

	//(9)
	void findAllTests(time_point<system_clock> from, time_point<system_clock> to);

	//(10)
	void findSickPeopleInDistrict(unsigned int id, time_point<system_clock> from, time_point<system_clock> to);

	//(11)
	void findSickPeopleInDistrictOrderByTestValue(unsigned int id, time_point<system_clock> from, time_point<system_clock> to);

	//(12)
	void findSickPeopleInRegion(const unsigned int regionId, time_point<system_clock> from, time_point<system_clock> to);

	//(13)
	void findSickPeople(time_point<system_clock> from, time_point<system_clock> to);

	//(14)
	void findMostSickPersonInDistrict(time_point<system_clock> from, time_point<system_clock> to);

	//(15)
	void findDistrictsOrderedBySickPeopleCount(time_point<system_clock> from, time_point<system_clock> to);

	//(16)
	void findRegionsOrderedBySickPeopleCount(time_point<system_clock> from, time_point<system_clock> to);

	//(17)
	void findAllTestsAtWorkplace(int workplaceId, time_point<system_clock> from, time_point<system_clock> to);

	//(18)
	void findTest(unsigned int testId);

	//(19)
	void insert(std::string birthNumber, std::string firstName, std::string lastName, year_month_day birthDay);

	//(20)
	void removeTest(unsigned int testId);

	//(21)
	void removePerson(std::string birthNumber);
	
	void printAllData();

	~Presenter() = default;
};