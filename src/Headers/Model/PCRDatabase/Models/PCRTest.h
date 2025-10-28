#pragma once
#include <chrono>
#include <string>
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

public:
	PCRTest(unsigned int testId, unsigned int workplaceId, unsigned int districtId, unsigned int regionId, bool result,
		double testValue, std::string note, std::chrono::time_point<std::chrono::system_clock> testDate, std::string birthNumber);
	inline unsigned int testId();
	inline unsigned int workplaceId();
	inline unsigned int districtId();
	inline unsigned int regionId();
	inline bool result();
	inline double testValue();
	inline std::string note();
	inline std::chrono::time_point<std::chrono::system_clock> testDate();
	inline std::string birthNumber();
	inline void print();
	~PCRTest() = default;
};