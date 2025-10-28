#include "../../../../Headers/Model/Tests/TestObjects/Number.h"

Number::Number(int data)
{
	m_data = data;
}

int Number::getData()
{
	return m_data;
}

void Number::print()
{
	std::cout << m_data << "\n";
}

int Number::compare(IComparable* other) const
{
	int otherData = static_cast<Number*>(other)->getData();

	if (m_data == otherData)
	{
		return 0;
	}
	else if (m_data > otherData)
	{
		return -1;
	}
	else if (m_data < otherData)
	{
		return 1;
	}
}
