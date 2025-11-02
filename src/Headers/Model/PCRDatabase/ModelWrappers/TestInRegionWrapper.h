#pragma once
#include "TestWrapper.h"

class TestInRegionWrapper : public TestWrapper
{
public:
	TestInRegionWrapper(PCRTest* pcrTest, PersonWrapper* person = nullptr) : TestWrapper(pcrTest, person) {}
	int compare(IComparable* other) const override;
	void writeLine() override;
	IStorable* loadLine() override;
	~TestInRegionWrapper() = default;
};