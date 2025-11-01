#pragma once
#include <string>
#include <thread>
#include <mutex>
#include "../Model/PCRDatabase/Database.h"
#include "Commands/FindTestResultCommand.h"
#include "Commands/InsertCommand.h"
#include "Commands/GenerateCommand.h"
#include "Commands/PrintAllDataCommand.h"

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

	std::string m_output;
	std::string m_recordCount;

	void execute(std::function<void(std::string& output, std::string& recordCount)> callback);

public:
	Presenter() : m_resultCommand(&m_database), m_insertCommand(&m_database), m_generateCommand(&m_database), m_isExecuting(false),
				  m_printAllDataCommand(&m_database)
	{};
	
	inline bool isExecuting() { return m_isExecuting; }

	void setOutput(std::string output, std::string recordCount);
	
	std::pair<std::string, std::string> output();

	void generatePeople(int count);

	void generateTests(int count);

	void insert(std::string birthNumber, std::string firstName, std::string lastName, std::chrono::year_month_day birthDay);
	
	void insert(unsigned int testId, unsigned int workplaceId, unsigned int districtId, 
				unsigned int regionId, bool result, double testValue, std::string note,
				std::chrono::time_point<std::chrono::system_clock> testDate, std::string birthNumber);

	void printAllData();
	
	void findResultByPatientAndTestId(unsigned int testId, std::string birthNumber, bool printPerson);
	
	~Presenter() = default;
};