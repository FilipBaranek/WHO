#include "../../../../Headers/Model/PCRDatabase/Models/PCRTest.h"

PCRTest::PCRTest(unsigned int testId, unsigned int workplaceId, unsigned int districtId, unsigned int regionId, bool result,
	double testValue, std::string note, std::chrono::time_point<std::chrono::system_clock> testDate, std::string birthNumber)
{
	m_testId = testId;
	m_workplaceId = workplaceId;
	m_districtId = districtId;
	m_regionId = regionId;
	m_result = result;
	m_testValue = testValue;
	m_note = note;
	m_testDate = testDate;
	m_birthNumber = birthNumber;
}

inline unsigned int PCRTest::testId()
{
	return m_testId;
}

inline unsigned int PCRTest::workplaceId()
{
	return m_workplaceId;
}

inline unsigned int PCRTest::districtId()
{
	return m_districtId;
}

inline unsigned int PCRTest::regionId()
{
	return m_regionId;
}

inline bool PCRTest::result()
{
	return m_result;
}

inline double PCRTest::testValue()
{
	return m_testValue;
}

inline std::string PCRTest::note()
{
	return m_note;
}

inline std::chrono::time_point<std::chrono::system_clock> PCRTest::testDate()
{
	return m_testDate;
}

inline std::string PCRTest::birthNumber()
{
	return m_birthNumber;
}

inline void PCRTest::print()
{
	std::cout << "[" << m_testId << "] Birth number :" << m_birthNumber << ", Result: " << m_result <<
		", Test value: " << m_testValue << ", Workplace: " << m_workplaceId << ", District: " << m_districtId <<
		", Region: " << m_regionId << ", Date: " << m_testDate << ", Note: " << m_note << "\n";
}
