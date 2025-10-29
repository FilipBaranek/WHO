#include "../../../../Headers/Model/PCRDatabase/ModelWrappers/TestWrapper.h"

TestWrapper::TestWrapper(PCRTest* pcrTest)
{
	m_pcrTest = pcrTest;
}

PCRTest* TestWrapper::getData()
{
	return m_pcrTest;
}

int TestWrapper::compare(IComparable* other) const
{
	TestWrapper* otherTest = static_cast<TestWrapper*>(other);

	if (m_pcrTest->birthNumber() > otherTest->getData()->birthNumber())
	{
		return -1;
	}
	else if (m_pcrTest->birthNumber() < otherTest->getData()->birthNumber())
	{
		return 1;
	}
	else
	{
		if (m_pcrTest->testId() > otherTest->getData()->testId())
		{
			return -1;
		}
		else if (m_pcrTest->testId() < otherTest->getData()->testId())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	return 0;
}
