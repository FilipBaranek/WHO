#include "../../../../Headers/Model/HeapFile/Helpers/ByteConverter.h"

void ByteConverter::toByteFromString(BinaryString& string, std::vector<uint8_t>& bytesOutput)
{
	toByteFromPrimitive<int>(string.m_capacity, bytesOutput);
	toByteFromPrimitive<int>(string.m_validBitCount, bytesOutput);
	for (int i{}; i < string.m_validBitCount; ++i)
	{
		toByteFromPrimitive<char>(string.m_str[i], bytesOutput);
	}
	for (int i = string.m_validBitCount; i < string.m_capacity; ++i)
	{
		char padding;
		toByteFromPrimitive<char>(padding, bytesOutput);
	}
}

BinaryString ByteConverter::fromByteToString(std::string& destString, uint8_t* byteBuffer)
{
	int capacity, validBitCount;
	std::memcpy(&capacity, byteBuffer, sizeof(capacity));
	std::memcpy(&validBitCount, byteBuffer + sizeof(capacity), sizeof(validBitCount));

	destString.assign(reinterpret_cast<const char*>(byteBuffer + sizeof(capacity) + sizeof(validBitCount)), validBitCount);

	return BinaryString(capacity, validBitCount, destString);
}
