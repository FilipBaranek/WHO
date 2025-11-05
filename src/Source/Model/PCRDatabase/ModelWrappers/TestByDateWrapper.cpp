#include "../../../../Headers/Model/PCRDatabase/ModelWrappers/TestByDateWrapper.h"
#include "../../../../Headers/Model/PCRDatabase/ModelWrappers/PersonWrapper.h"

int TestByDateWrapper::compare(IComparable* other) const
{
	TestByDateWrapper* otherTestWrapper = static_cast<TestByDateWrapper*>(other);
	PCRTest* otherTest = otherTestWrapper->getData();

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
		if (m_pcrTest->testId() > otherTest->testId())
		{
			return -1;
		}
		else if (m_pcrTest->testId() < otherTest->testId())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}