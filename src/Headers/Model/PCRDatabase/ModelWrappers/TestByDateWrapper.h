#pragma once
#include "TestWrapper.h"

class TestByDateWrapper : public TestWrapper
{
public:
	TestByDateWrapper(PCRTest* pcrTest, PersonWrapper* person = nullptr) : TestWrapper(pcrTest, person) {}
	int compare(IComparable* other) const override;
	void writeLine() override;
	IStorable* loadLine() override;
	~TestByDateWrapper() = default;
};