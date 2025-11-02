#pragma once
#include <string>
#include "Command.h"

class FindPatientsTestsCommand : public Command
{
private:
	std::string m_birthNumber;

public:
	FindPatientsTestsCommand(Database* database) : Command(database) {}
	inline void setParams(std::string birthNumber) { m_birthNumber = birthNumber; };
	void execute(std::string& output, std::string& recordCount) override;
	~FindPatientsTestsCommand() override = default;
};