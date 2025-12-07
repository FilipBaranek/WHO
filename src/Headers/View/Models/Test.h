#pragma once
#include <string>
#include <chrono>


class TestView
{
private:
	unsigned int m_testId;
	bool m_result;
	double m_testValue;
	std::chrono::time_point<std::chrono::system_clock> m_testDate;
	std::string m_note;

public:
	TestView(unsigned int testId, bool result, double testValue, std::string note, std::chrono::time_point<std::chrono::system_clock> testDate) :
		m_testId(testId), m_result(result), m_testValue(testValue), m_note(note), m_testDate(testDate)
	{};

	inline unsigned int testId() { return m_testId; }

	inline bool result() { return m_result; }

	inline double testValue() { return m_testValue; }

	inline std::string note() { return m_note; }

	inline std::chrono::time_point<std::chrono::system_clock> testDate() { return m_testDate; }

	~TestView() = default;
};