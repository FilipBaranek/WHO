#pragma once
#include <string>
#include "../Command.h"
#include "../../../../Headers/Model/Tests/FunctionalityTester.h"

class FunctionalityTestCommand : public Command
{
private:
	unsigned int& m_pregeneratedDataCount;
	std::string& m_output;

public:
	FunctionalityTestCommand(unsigned int& pregeneratedDataCount, std::string& output) : m_pregeneratedDataCount(pregeneratedDataCount), m_output(output) {};
	void execute() override;
	~FunctionalityTestCommand() override = default;
};