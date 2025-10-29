#pragma once
#include <chrono>
#include <string>
#include <sstream>
#include <iostream>

class PCRTest
{
private:
	unsigned int m_testId;
	unsigned int m_workplaceId;
	unsigned int m_districtId;
	unsigned int m_regionId;
	bool m_result;
	double m_testValue;
	std::string m_note;
	std::chrono::time_point<std::chrono::system_clock> m_testDate;
	std::string m_birthNumber;
	std::ostringstream m_oss;

public:
	PCRTest(unsigned int testId, unsigned int workplaceId, unsigned int districtId, unsigned int regionId, bool result,
			double testValue, std::string note, std::chrono::time_point<std::chrono::system_clock> testDate, std::string birthNumber) :
			m_testId(testId), m_workplaceId(workplaceId), m_districtId(m_districtId), m_regionId(regionId), m_result(result),
			m_testValue(testValue), m_note(note), m_testDate(testDate), m_birthNumber(birthNumber) 
	{};

	inline unsigned int testId() { return m_testId; }
	inline unsigned int workplaceId() { return m_workplaceId; }
	inline unsigned int districtId() { return m_districtId; }
	inline unsigned int regionId() { return m_regionId; }
	inline bool result() { return m_result; }
	inline double testValue() { return m_testValue; }
	inline std::string note() { return m_note; }
	inline std::chrono::time_point<std::chrono::system_clock> testDate() { return m_testDate; }
	inline std::string birthNumber() { return m_birthNumber; }

	inline std::string toString()
	{
		std::time_t time = std::chrono::system_clock::to_time_t(m_testDate);
		std::tm tm = *std::localtime(&time);

		m_oss.clear();
		m_oss << "[" << m_testId << "] Birth number :" << m_birthNumber << ", Result: " << m_result
			<< ", Test value: " << m_testValue << ", Workplace: " << m_workplaceId << ", District: "
			<< m_districtId << ", Region: " << m_regionId << ", Date: " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << ", Note: " << m_note << "\n";
		return m_oss.str();
	}

	~PCRTest() = default;
};