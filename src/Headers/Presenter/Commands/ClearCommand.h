#pragma once
#include <string>
#include "Command.h"

class ClearCommand : public Command
{
public:
	ClearCommand(Database* database) : Command(database) {}
	
	void execute(std::string& output, std::string& recordCount) override;
	
	~ClearCommand() = default;
};