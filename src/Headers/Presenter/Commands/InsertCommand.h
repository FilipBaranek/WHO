#pragma once
#include <string>
#include "Command.h"
#include "../../../Headers/Model/PCRDatabase/Models/Person.h"
#include "../../../Headers/Model/PCRDatabase/Models/PCRTest.h"

class InsertCommand : public Command
{
private:
	std::string& m_output;
	Person* m_person;
	PCRTest* m_test;

public:
	InsertCommand(Database* database, std::string& output) : Command(database), m_output(output) {}
	void setParams(std::string birthNumber, std::string firstName, std::string lastName, std::chrono::year_month_day birthDay);
	void setParams(unsigned int testId, unsigned int workplaceId, unsigned int districtId, unsigned int regionId, bool result, double testValue,
				   std::string note, std::chrono::time_point<std::chrono::system_clock> testDate, std::string birthNumber);
	void execute() override;
	~InsertCommand() override = default;
};