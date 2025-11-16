#pragma once
#include <string>
#include "BinaryString.h"
#include "../../Interfaces/IRecord.h"

class ByteConverter
{
public:
	template<typename T>
	static void toByteFromPrimitive(T& primitive, uint8_t* bytesOutput)
	{
		std::memcpy(bytesOutput, &primitive, sizeof(primitive));
	}

	template<typename T>
	static T fromByteToPrimitive(uint8_t* byteBuffer)
	{
		T primitive;
		std::memcpy(&primitive, byteBuffer, sizeof(T));
		return primitive;
	}

	static void toByteFromString(BinaryString& string, uint8_t* bytesOutput);
	static BinaryString fromByteToString(std::string& destString, uint8_t* byteBuffer);

	ByteConverter() = delete;
	~ByteConverter() = delete;
};
