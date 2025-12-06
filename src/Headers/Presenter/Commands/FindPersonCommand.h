#pragma once
#include <string>
#include "Command.h"

class FindPersonCommand : public Command
{
private:
	std::string m_birthNumber;

public:
	FindPersonCommand(Database* database) : Command(database) {}
	inline void setParams(std::string birthNumber) { m_birthNumber = birthNumber; };
	void execute(std::string& output, std::string& recordCount) override;
	~FindPersonCommand() = default;
};