#pragma once
#include <string>
#include <chrono>
#include "Command.h"
#include "../../View/Models/Test.h"

using namespace std::chrono;

class EditTestCommand : public Command
{
private:
	unsigned int m_testId;
	bool m_result;
	double m_testValue;
	std::string m_note;
	time_point<system_clock> m_testDate;

	TestView* m_test;

public:
	EditTestCommand(Database* database) : Command(database), m_test(nullptr), m_testId(-1) {}

	inline void setParams(unsigned int testId) { m_testId = testId; }

	inline void setParams(unsigned int testId, bool result, double testValue, std::string note, time_point<system_clock> testDate)
	{
		m_testId = testId;
		m_result = result;
		m_testValue = testValue;
		m_note = note;
		m_testDate = testDate;
	}

	void execute(std::string& output, std::string& recordCount) override;

	void executeEdit(std::string& output, std::string& recordCount);

	inline TestView* test()
	{
		TestView* output = m_test;
		m_test = nullptr;
		return output;
	}

	~EditTestCommand() = default;
};