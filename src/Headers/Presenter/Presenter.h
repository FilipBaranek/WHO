#pragma once
#include <string>
#include "../Model/PCRDatabase/Database.h"
#include "./Commands/FindTestResultCommand.h"
#include "./Commands/InsertCommand.h"

class Presenter
{
private:
	std::string& m_output;
	Database m_database;
	FindTestResultCommand m_resultCommand;
	InsertCommand m_insertCommand;

public:
	Presenter(std::string& output) : m_output(output), m_resultCommand(&m_database, m_output), m_insertCommand(&m_database, m_output) {};
	void insert(std::string birthNumber, std::string firstName, std::string lastName, std::chrono::year_month_day birthDay);
	void insert(unsigned int testId, unsigned int workplaceId, unsigned int districtId, unsigned int regionId, bool result, double testValue,
				std::string note, std::chrono::time_point<std::chrono::system_clock> testDate, std::string birthNumber);
	void findResultByPatientAndTestId(unsigned int testId, std::string birthNumber, bool printPerson);
	~Presenter() = default;
};