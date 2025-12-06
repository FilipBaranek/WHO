#pragma once
#include <string>
#include <thread>
#include <mutex>
#include <functional>
#include <chrono>
#include "Commands/GenerateCommand.h"
#include "Commands/PrintAllDataCommand.h"
#include "Commands/InsertCommand.h"
#include "Commands/FindTestCommand.h"

using namespace std::chrono;

class Presenter
{
protected:
	Database* m_database;
	InsertCommand m_insertCommand;

private:
	bool m_isExecuting;
	std::mutex m_outputMutex;
	std::string m_output;
	std::string m_recordCount;

	GenerateCommand m_generateCommand;
	PrintAllDataCommand m_printAllDataCommand;
	FindTestCommand m_findTestCommand;

protected:
	void execute(std::function<void(std::string& output, std::string& recordCount)> callback);

public:
	Presenter(Database* database) : m_isExecuting(false), m_database(database),
		m_generateCommand(database), m_printAllDataCommand(database), m_insertCommand(database),
		m_findTestCommand(m_database)
	{}
	
	inline bool isExecuting() { return m_isExecuting; }

	void setOutput(std::string output, std::string recordCount);
	
	std::pair<std::string, std::string> output();

	void generatePeople(int count);

	void generateTests(int count);
	
	virtual void insert(unsigned int testId, unsigned int workplaceId, unsigned int districtId,
						unsigned int regionId, bool result, double testValue, std::string note,
						time_point<system_clock> testDate, std::string birthNumber) = 0;

	virtual void insert(std::string birthNumber, std::string firstName, std::string lastName, year_month_day birthDay) = 0;

	void findTest(unsigned int testId);

	void printAllData();

	virtual ~Presenter() = default;
};