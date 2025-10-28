#pragma once
#include "../Command.h"

class FunctionalityTestCommand : public Command
{
private:
	const char* m_output;

public:
	FunctionalityTestCommand() = default;
	void execute() override;
	~FunctionalityTestCommand() override = default;
};