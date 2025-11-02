#include "../../../../Headers/Model/PCRDatabase/ModelWrappers/TestInRegionWrapper.h"

int TestInRegionWrapper::compare(IComparable* other) const
{
	TestInRegionWrapper* otherTestWrapper = static_cast<TestInRegionWrapper*>(other);
	PCRTest* otherTest = otherTestWrapper->getData();

	if (m_pcrTest->regionId() > otherTest->regionId())
	{
		return -1;
	}
	else if (m_pcrTest->regionId() < otherTest->regionId())
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

void TestInRegionWrapper::writeLine()
{
}

IStorable* TestInRegionWrapper::loadLine()
{
	return nullptr;
}
