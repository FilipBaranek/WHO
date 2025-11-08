#pragma once
#include <string>
#include "Command.h"

class GenerateCommand : public Command
{
private:
	int m_peopleCount;
	int m_testCount;

public:
	GenerateCommand(Database* database) : Command(database), m_peopleCount(0), m_testCount(0) {}
	inline void setPeopleParams(int count) { m_peopleCount = count; };
	inline void setTestParams(int count) { m_testCount = count; };
	void execute(std::string& output, std::string& recordCount) override;
	~GenerateCommand() = default;
};