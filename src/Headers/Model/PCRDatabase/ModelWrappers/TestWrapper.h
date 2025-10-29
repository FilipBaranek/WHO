#pragma once
#include "../../Interfaces/IComparable.h"
#include "../Models/PCRTest.h"


class TestWrapper : public IComparable
{
private:
	PCRTest* m_pcrTest;

public:
	TestWrapper(PCRTest* person);
	PCRTest* getData();
	int compare(IComparable* other) const override;
	~TestWrapper() = default;
};