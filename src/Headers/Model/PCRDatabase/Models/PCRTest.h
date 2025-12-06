#pragma once
#include <chrono>
#include <string>
#include <sstream>
#include <iostream>
#include "ReducedPCRTest.h"

//Forward declaration - cycle include
class Person;

class PCRTest : public ReducedPCRTest
{
private:
	unsigned int m_workplaceId;
	unsigned int m_districtId;
	unsigned int m_regionId;
	std::string m_birthNumber;

public:
	PCRTest(unsigned int testId, unsigned int workplaceId, unsigned int districtId, unsigned int regionId, bool result, double testValue,
			std::string note, std::chrono::time_point<std::chrono::system_clock> testDate, std::string birthNumber) :
			ReducedPCRTest(testId, result, testValue, note, testDate),
			m_workplaceId(workplaceId), m_districtId(districtId), m_regionId(regionId), m_birthNumber(birthNumber)
	{};

	inline void setTestId(unsigned int id) { this->m_testId = id; }
	
	inline unsigned int workplaceId() { return m_workplaceId; }
	
	inline unsigned int districtId() { return m_districtId; }
	
	inline unsigned int regionId() { return m_regionId; }

	inline std::string birthNumber() { return m_birthNumber; }

	std::string toCsvFormat() override;

	std::string toString() override;

	~PCRTest() = default;
};