#pragma once
#include <string>
#include "Command.h"
#include "../Enums/RemoveType.h"

class RemoveCommand : public Command
{
private:
	REMOVETYPE m_removeType;
	int m_testId;
	std::string m_birthNumber;

public:
	RemoveCommand(Database* database) : Command(database), m_testId(-1) {}
	inline void setParams(REMOVETYPE removeType) { m_removeType = removeType; }
	inline void setParams(int testId) { m_testId = testId; }
	inline void setParams(std::string birthNumber) { m_birthNumber = birthNumber; }
	void execute(std::string& output, std::string& recordCount) override;
	~RemoveCommand() = default;
};