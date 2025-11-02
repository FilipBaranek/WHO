#pragma once
#include <string>
#include <chrono>
#include "Command.h"

class FindAllTestsByLocationIdCommand : public Command
{
private:
	const int DISTRICT = 1;
	const int REGION = 2;
	const int COUNTRY = 3;

	int m_location;
	int m_id;
	std::chrono::time_point<std::chrono::system_clock> m_from;
	std::chrono::time_point<std::chrono::system_clock> m_to;

public:
	FindAllTestsByLocationIdCommand(Database* database) : Command(database), m_location(0), m_id(0) {}

	inline void setParams(int location, int id,
		std::chrono::time_point<std::chrono::system_clock> from,
		std::chrono::time_point<std::chrono::system_clock> to)
	{
		m_location = location;
		m_id = id;
		m_from = from;
		m_to = to;
	};

	void execute(std::string& output, std::string& recordCount) override;

	~FindAllTestsByLocationIdCommand() override = default;
};