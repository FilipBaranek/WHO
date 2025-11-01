#pragma once
#include <string>
#include "../../Model/PCRDatabase/Database.h"

class Command
{
protected:
	Database* m_database;

public:
	Command(Database* database) : m_database(database) {};
	virtual void execute(std::string& output, std::string& recordCount) = 0;
	virtual ~Command() = default;
};