#pragma once
#include <string>
#include <chrono>
#include "Command.h"

class FindAllTestsInDistrictCommand : public Command
{
private:
	int m_districtId;
	std::chrono::time_point<std::chrono::system_clock> m_from;
	std::chrono::time_point<std::chrono::system_clock> m_to;

public:
	FindAllTestsInDistrictCommand(Database* database) : Command(database), m_districtId(0) {}

	inline void setParams(int districtId,
		std::chrono::time_point<std::chrono::system_clock> from,
		std::chrono::time_point<std::chrono::system_clock> to)
	{
		m_districtId = districtId;
		m_from = from;
		m_to = to;
	};

	void execute(std::string& output, std::string& recordCount) override;

	~FindAllTestsInDistrictCommand() override = default;
};