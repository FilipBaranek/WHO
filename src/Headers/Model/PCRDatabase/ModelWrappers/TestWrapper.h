#pragma once
#include "../../Interfaces/IComparable.h"
#include "../Models/PCRTest.h"


class TestWrapper : public IComparable
{
private:
	PCRTest* m_pcrTest;

public:
	TestWrapper(PCRTest* pcrTest) : m_pcrTest(pcrTest) {}
	inline PCRTest* getData() { return m_pcrTest; }
	int compare(IComparable* other) const override;
	~TestWrapper() = default;
};