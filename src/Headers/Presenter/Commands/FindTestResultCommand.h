#pragma once
#include <string>
#include <utility>
#include "Command.h"

class FindTestResultCommand : public Command
{
private:
	unsigned int m_testId;
	std::string m_birthNumber;

public:
	FindTestResultCommand(Database* database) : Command(database) {}
	inline void setParams(unsigned int testId, std::string birthNumber) { m_testId = testId; m_birthNumber = birthNumber; };
	void execute(std::string& output, std::string& recordCount) override;
	~FindTestResultCommand() override = default;
};