#pragma once
#include <iostream>
#include "../../Interfaces/IComparable.h"

class Number : public IComparable
{
private:
	int m_data;

public:
	Number(int data);
	int getData();
	void print();
	inline int compare(IComparable* other) const override;
	~Number() = default;
};

struct NumberRBComparator
{
	bool operator()(Number* a, Number* b) const
	{
		return a->getData() < b->getData();
	}
};

