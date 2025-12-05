#include "../../../../Headers/Model/PCRDatabase/Models/ReducedPCRTest.h"

std::string ReducedPCRTest::toCsvFormat()
{
	std::time_t time = std::chrono::system_clock::to_time_t(m_testDate);
	std::tm tm = *std::localtime(&time);
	std::ostringstream oss;
	oss << m_testId << ";" << (m_result ? 1 : 0) << ";" << m_testValue << ";"
		<< std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << ";" << (m_note.empty() ? ";" : m_note) << "\n";
	return oss.str();
}

std::string ReducedPCRTest::toString()
{
	std::time_t time = std::chrono::system_clock::to_time_t(m_testDate);
	std::tm tm = *std::localtime(&time);

	std::ostringstream oss;
	oss << "[" << m_testId << "]\n" << "Result: " << (m_result ? "Pozitivny" : "Negativny")
		<< "\nTest value: " << m_testValue << "\nDate: " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "\nNote: " << m_note << "\n";
	return oss.str();
}