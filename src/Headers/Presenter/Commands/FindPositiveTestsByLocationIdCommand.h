#pragma once
#include <string>
#include <chrono>
#include <utility>
#include "Command.h"
#include "../Enums/Location.h"

using namespace std::chrono;

class FindPositiveTestsByLocationIdCommand : public Command
{
private:
	SEARCHTYPE m_searchType;
	LOCATION m_location;
	bool m_orderBy;
	int m_id;
	time_point<system_clock> m_from;
	time_point<system_clock> m_to;

public:
	FindPositiveTestsByLocationIdCommand(Database* database) : Command(database), m_id(0) {}
	
	inline void setParams(SEARCHTYPE searchType) { m_searchType = searchType; }

	inline void setParams(LOCATION location) { m_location = location; }

	inline void setParams(time_point<system_clock> from, time_point<system_clock> to, int id = -1, bool orderBy = false)
	{
		m_id = id;
		m_from = from;
		m_to = to;
		m_orderBy = orderBy;
	};
	
	void execute(std::string& output, std::string& recordCount) override;
	
	~FindPositiveTestsByLocationIdCommand() override = default;
};