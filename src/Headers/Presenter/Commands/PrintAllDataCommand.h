#pragma once
#include <string>
#include "Command.h"

class PrintAllDataCommand : public Command
{
public:
	PrintAllDataCommand(Database* database) : Command(database) {}
	void execute(std::string& output, std::string& recordCount) override;
	~PrintAllDataCommand() override = default;
};