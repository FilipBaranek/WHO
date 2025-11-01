#pragma once
#include <string>
#include "Command.h"

class FindTestResultCommand : public Command
{
private:
	bool m_printPerson;
	unsigned int m_testId;
	std::string m_birthNumber;

public:
	FindTestResultCommand(Database* database) : Command(database) {}
	void setParams(unsigned int testId, std::string birthNumber, bool printPerson = true);
	void execute(std::string& output, std::string& recordCount) override;
	~FindTestResultCommand() override = default;
};