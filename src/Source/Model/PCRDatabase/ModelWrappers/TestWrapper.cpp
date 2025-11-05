#include "../../../../Headers/Model/PCRDatabase/ModelWrappers/TestWrapper.h"
#include "../../../../Headers/Model/PCRDatabase/ModelWrappers/PersonWrapper.h"

int TestWrapper::compare(IComparable* other) const
{
	TestWrapper* otherTestWrapper = static_cast<TestWrapper*>(other);
	PCRTest* otherTest = otherTestWrapper->getData();

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

std::string TestWrapper::writeLine()
{
	return m_pcrTest->toCsvFormat();
}

IStorable* TestWrapper::loadLine()
{
	return nullptr;
}
