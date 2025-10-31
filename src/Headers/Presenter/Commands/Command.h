#pragma once
#include "../../Model/PCRDatabase/Database.h"

class Command
{
protected:
	Database* m_database;

public:
	Command(Database* database) : m_database(database) {};
	virtual void execute() = 0;
	virtual ~Command() = default;
};