#include "../../../../Headers/Model/PCRDatabase/ModelWrappers/TestInDistrictWrapper.h"

int TestInDistrictWrapper::compare(IComparable* other) const
{
	TestInDistrictWrapper* otherTestWrapper = static_cast<TestInDistrictWrapper*>(other);
	PCRTest* otherTest = otherTestWrapper->getData();

	if (m_pcrTest->districtId() > otherTest->districtId())
	{
		return -1;
	}
	else if (m_pcrTest->districtId() < otherTest->districtId())
	{
		return 1;
	}
	else
	{
		if (m_pcrTest->testDate() > otherTest->testDate())
		{
			return -1;
		}
		else if (m_pcrTest->testDate() < otherTest->testDate())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

void TestInDistrictWrapper::writeLine()
{
}

IStorable* TestInDistrictWrapper::loadLine()
{
	return nullptr;
}
