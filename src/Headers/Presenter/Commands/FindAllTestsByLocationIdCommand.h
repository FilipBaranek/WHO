#pragma once
#include <string>
#include <chrono>
#include <utility>
#include "Command.h"
#include "../Enums/Location.h"

using namespace std::chrono;

class FindAllTestsByLocationIdCommand : public Command
{
private:
	LOCATION m_location;
	int m_id;
	time_point<system_clock> m_from;
	time_point<system_clock> m_to;

public:
	FindAllTestsByLocationIdCommand(Database* database) : Command(database), m_id(0) {}

	inline void setParams(LOCATION location, time_point<system_clock> from, time_point<system_clock> to, int id = -1)
	{
		m_location = location;
		m_id = id;
		m_from = from;
		m_to = to;
	};

	void execute(std::string& output, std::string& recordCount) override;

	~FindAllTestsByLocationIdCommand() = default;
};