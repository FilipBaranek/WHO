#pragma once
#include <string>
#include "../Command.h"

class SpeedTestCommand : public Command
{
private:
	bool& m_dataInOrder;
	std::string& m_output;

public:
	SpeedTestCommand(std::string& output, bool& dataInOrder) : m_output(output), m_dataInOrder(dataInOrder) {};
	void execute() override;
	~SpeedTestCommand() override = default;
};
