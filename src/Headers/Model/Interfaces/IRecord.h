#pragma once
#include <cstdint>
#include <vector>
#include <string>

class IRecord
{
public:
	IRecord() = default;

	virtual IRecord* clone() = 0;

	virtual bool equals(IRecord* other) = 0;

	virtual bool is(IRecord* other) = 0;

	virtual int getSize() = 0;

	virtual std::string toString() = 0;

	virtual bool toBytes(uint8_t* bytesOutput) = 0;

	virtual IRecord* fromBytes(uint8_t* byteBuffer) = 0;

	virtual int hash() = 0;

	virtual ~IRecord() = default;
};