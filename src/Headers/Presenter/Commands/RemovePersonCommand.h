#pragma once
#include <string>
#include "Command.h"

class RemovePersonCommand : public Command
{
private:
	std::string m_birthNumber;

public:
	RemovePersonCommand(Database* database) : Command(database) {}
	inline void setParams(std::string birthNumber) { m_birthNumber = birthNumber; };
	void execute(std::string& output, std::string& recordCount) override;
	~RemovePersonCommand() override = default;
};