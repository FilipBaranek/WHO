#pragma once
#include "TestWrapper.h"

class TestInDistrictWrapper : public TestWrapper
{
public:
	TestInDistrictWrapper(PCRTest* pcrTest, PersonWrapper* person = nullptr) : TestWrapper(pcrTest, person) {}
	int compare(IComparable* other) const override;
	void writeLine() override;
	IStorable* loadLine() override;
	~TestInDistrictWrapper() = default;
};