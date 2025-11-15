#pragma once
#include <vector>
#include <string>
#include "BinaryString.h"
#include "../../Interfaces/IRecord.h"

class ByteConverter
{
public:
	template<typename T>
	static void toByteFromPrimitive(T& primitive, std::vector<uint8_t>& bytesOutput)
	{
		uint8_t buffer[sizeof(primitive)];
		std::memcpy(buffer, &primitive, sizeof(primitive));
		bytesOutput.insert(bytesOutput.end(), buffer, buffer + sizeof(primitive));
	}

	template<typename T>
	static T fromByteToPrimitive(uint8_t* byteBuffer)
	{
		T primitive;
		std::memcpy(&primitive, byteBuffer, sizeof(T));
		return primitive;
	}

	static void toByteFromString(BinaryString& string, std::vector<uint8_t>& bytesOutput);
	static BinaryString fromByteToString(std::string& destString, uint8_t* byteBuffer);

	ByteConverter() = delete;
	~ByteConverter() = delete;
};
