#pragma once
#include <string>

struct BinaryString
{
	int m_capacity;
	int m_validBitCount;
	std::string& m_str;

	int size()
	{
		return sizeof(m_capacity) + sizeof(m_validBitCount) + m_capacity;
	}
};