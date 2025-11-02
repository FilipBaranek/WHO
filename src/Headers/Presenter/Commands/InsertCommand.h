#pragma once
#include <string>
#include "Command.h"
#include "../../../Headers/Model/PCRDatabase/ModelWrappers/PersonWrapper.h"
#include "../../../Headers/Model/PCRDatabase/ModelWrappers/TestWrapper.h"

class InsertCommand : public Command
{
private:
	PersonWrapper* m_person;
	TestWrapper* m_test;

public:
	InsertCommand(Database* database) : Command(database), m_person(nullptr), m_test(nullptr) {}

	void setParams(std::string birthNumber, std::string firstName, std::string lastName, std::chrono::year_month_day birthDay);
	
	void setParams(unsigned int testId, unsigned int workplaceId, unsigned int districtId, unsigned int regionId, bool result, double testValue,
				   std::string note, std::chrono::time_point<std::chrono::system_clock> testDate, std::string birthNumber);
	
	void execute(std::string& output, std::string& recordCount) override;
	
	~InsertCommand() override = default;
};