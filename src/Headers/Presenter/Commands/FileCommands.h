#pragma once
#include <string>
#include <utility>
#include "Command.h"
#include "../Enums/FileCommandType.h"

using namespace std::chrono;

class FileCommands : public Command
{
private:
	COMMANDTYPE m_type;

public:
	FileCommands(Database* database) : Command(database) {}
	inline void setParams(COMMANDTYPE type) { m_type = type; }
	void execute(std::string& output, std::string& recordCount) override;
	~FileCommands() override = default;
};