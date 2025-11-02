#pragma once
#include <string>
#include "Command.h"

class RemoveTestCommand : public Command
{
private:
	int m_testId;

public:
	RemoveTestCommand(Database* database) : Command(database), m_testId(-1) {}
	inline void setParams(int testId) { m_testId = testId; };
	void execute(std::string& output, std::string& recordCount) override;
	~RemoveTestCommand() override = default;
};