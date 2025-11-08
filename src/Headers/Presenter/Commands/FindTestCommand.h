#pragma once
#include <string>
#include "Command.h"

class FindTestCommand : public Command
{
private:
	int m_testId;

public:
	FindTestCommand(Database* database) : Command(database), m_testId(0) {}
	inline void setParams(int testId) { m_testId = testId; };
	void execute(std::string& output, std::string& recordCount) override;
	~FindTestCommand() = default;
};