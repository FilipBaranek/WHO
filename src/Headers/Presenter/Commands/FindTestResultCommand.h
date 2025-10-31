#pragma once
#include <string>
#include "Command.h"

class FindTestResultCommand : public Command
{
private:
	bool m_printPerson;
	unsigned int m_testId;
	std::string m_birthNumber;
	std::string& m_output;

public:
	FindTestResultCommand(Database* database, std::string& output) : Command(database), m_output(output) {}
	void setParams(unsigned int testId, std::string birthNumber, bool printPerson = true);
	void execute() override;
	~FindTestResultCommand() override = default;
};