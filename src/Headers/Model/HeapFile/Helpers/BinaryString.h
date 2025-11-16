#pragma once
#include <string>

struct BinaryString
{
	int m_capacity;
	int m_validBitCount;
	std::string& m_str;

	int size()
	{
		return 2 * sizeof(int) + m_capacity;
	}
};