#include "../../../../Headers/Model/HeapFile/Helpers/ByteConverter.h"

uint8_t* ByteConverter::toByteFromString(BinaryString& string, uint8_t* bytesOutput)
{
	uint8_t* index = bytesOutput;

	toByteFromPrimitive<int>(string.m_capacity, index);
	index += sizeof(string.m_capacity);

	toByteFromPrimitive<int>(string.m_validBitCount, index);
	index += sizeof(string.m_validBitCount);

	for (int i{}; i < string.m_validBitCount; ++i)
	{
		toByteFromPrimitive<char>(string.m_str[i], index);
		++index;
	}

	return index + (string.m_capacity - string.m_validBitCount);
}

BinaryString ByteConverter::fromByteToString(std::string& destString, uint8_t* byteBuffer)
{
	uint8_t* index = byteBuffer;

	int capacity, validBitCount;
	std::memcpy(&capacity, index, sizeof(capacity));
	index += sizeof(capacity);
	std::memcpy(&validBitCount, index, sizeof(validBitCount));
	index += sizeof(validBitCount);

	destString.assign(reinterpret_cast<const char*>(index), validBitCount);

	return BinaryString(capacity, validBitCount, destString);
}
